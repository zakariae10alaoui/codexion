#include "codexion.h"

static void lock_dongles_in_order(s_dongle *d1, s_dongle *d2)
{
    if (d1 == d2)
    {
        pthread_mutex_lock(&d1->mutex);
        return;
    }
    if (d1 < d2)
    {
        pthread_mutex_lock(&d1->mutex);
        pthread_mutex_lock(&d2->mutex);
    }
    else
    {
        pthread_mutex_lock(&d2->mutex);
        pthread_mutex_lock(&d1->mutex);
    }
}

static void unlock_dongles_in_order(s_dongle *d1, s_dongle *d2)
{
    if (d1 == d2)
    {
        pthread_mutex_unlock(&d1->mutex);
        return;
    }
    pthread_mutex_unlock(&d1->mutex);
    pthread_mutex_unlock(&d2->mutex);
}

static void update_cooldown(s_dongle *dongle, s_coder *coder)
{
    if (dongle->in_cooldown
        && get_time() >= dongle->last_released + coder->sim->dongle_cooldown)
        dongle->in_cooldown = 0;
}

static int try_take_both_dongles(s_coder *coder, s_dongle *d1, s_dongle *d2)
{
    update_cooldown(d1, coder);
    if (d1 == d2)
    {
        if (d1->is_free && !d1->in_cooldown && coder->id == d1->nodes[0].coder_id)
        {
            d1->is_free = 0;
            pop_from_queue(d1);
            return (1);
        }
        return (0);
    }
    update_cooldown(d2, coder);
    if (d1->is_free && !d1->in_cooldown && coder->id == d1->nodes[0].coder_id
        && d2->is_free && !d2->in_cooldown && coder->id == d2->nodes[0].coder_id)
    {
        d1->is_free = 0;
        d2->is_free = 0;
        pop_from_queue(d1);
        pop_from_queue(d2);
        return (1);
    }
    return (0);
}

static void wait_on_dongle_signal(s_dongle *dongle)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += 2000000;
    if (ts.tv_nsec >= 1000000000)
    {
        ts.tv_sec += 1;
        ts.tv_nsec -= 1000000000;
    }
    pthread_mutex_lock(&dongle->mutex);
    pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
    pthread_mutex_unlock(&dongle->mutex);
}

int handle_cooldown(s_dongle *dongle, s_coder *coder)
{
    struct timespec ts;
    long long target_time;

    if (!dongle->in_cooldown)
        return (1);
    
    target_time = dongle->last_released + coder->sim->dongle_cooldown;
    
    if (get_time() >= target_time)
    {
        dongle->in_cooldown = 0; 
        return (1);
    }

    ts.tv_sec = target_time / 1000;
    ts.tv_nsec = (target_time % 1000) * 1000000;

    pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);

    if (get_time() >= target_time)
    {
        dongle->in_cooldown = 0;
    }

    return (0); 
}
int sim_done(s_coder *coder, s_dongle *dongle)
{
    pthread_mutex_lock(&coder->sim->sim_lock);
    if (coder->sim->simulation_ended)
    {
        pthread_mutex_unlock(&dongle->mutex);
        pthread_mutex_unlock(&coder->sim->sim_lock);
        return (1);
    }
    pthread_mutex_unlock(&coder->sim->sim_lock);
    return (0);
}
int take_single_dongle(s_dongle *dongle,s_coder *coder)
{
    pthread_mutex_lock(&dongle->mutex);
    if (dongle->is_free && (!dongle->in_cooldown))
    {
        dongle->is_free = 0;
        pthread_mutex_unlock(&dongle->mutex);
        return 1;
    }
    register_in_queue(dongle, coder);
    while (!dongle->is_free || dongle->in_cooldown || coder->id != dongle->nodes[0].coder_id)
    {
        if(sim_done(coder,dongle))
            return 0;
        if (dongle->is_free)
        {
            if(!handle_cooldown(dongle, coder))
                continue;
            if(dongle->is_free && !dongle->in_cooldown && coder->id == dongle->nodes[0].coder_id)
                break;
        }        
        pthread_cond_wait(&dongle->cond, &dongle->mutex);
    }
    dongle->is_free = 0; 
    pop_from_queue(dongle);
    pthread_mutex_unlock(&dongle->mutex);
    return 1;
}
int take_dongles(s_coder *coder)
{
    lock_dongles_in_order(coder->first_dongle, coder->second_dongle);
    register_in_queue(coder->first_dongle, coder);
    if (coder->first_dongle != coder->second_dongle)
        register_in_queue(coder->second_dongle, coder);
    unlock_dongles_in_order(coder->first_dongle, coder->second_dongle);

    while (1)
    {
        lock_dongles_in_order(coder->first_dongle, coder->second_dongle);
        if (check_sim(coder))
        {
            unlock_dongles_in_order(coder->first_dongle, coder->second_dongle);
            return (0);
        }
        if (try_take_both_dongles(coder, coder->first_dongle, coder->second_dongle))
        {
            unlock_dongles_in_order(coder->first_dongle, coder->second_dongle);
            print_coder_status(coder, "has taken a dongle");
            print_coder_status(coder, "has taken a dongle");
            return (1);
        }
        unlock_dongles_in_order(coder->first_dongle, coder->second_dongle);
        wait_on_dongle_signal(coder->first_dongle);
        if (coder->first_dongle != coder->second_dongle)
            wait_on_dongle_signal(coder->second_dongle);
    }
}

void compile(s_coder *coder)
{
    if (take_dongles(coder))
    {
        pthread_mutex_lock(&coder->sim->sim_lock);
        if (coder->sim->simulation_ended)
        {
            release_dongles(coder->first_dongle, coder->second_dongle);
            pthread_mutex_unlock(&coder->sim->sim_lock);
            return;
        }
        pthread_mutex_unlock(&coder->sim->sim_lock);
        print_coder_status(coder, "is compiling");
        pthread_mutex_lock(&coder->lock);
        coder->last_compile_time = get_time();
        pthread_mutex_unlock(&coder->lock);
        usleep(coder->sim->time_to_compile * 1000);
        pthread_mutex_lock(&coder->lock);
        coder->compiles_done += 1;
        pthread_mutex_unlock(&coder->lock);
        release_dongles(coder->first_dongle, coder->second_dongle);
    }
}
