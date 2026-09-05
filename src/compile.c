#include "codexion.h"
int handle_cooldown(s_dongle *dongle, s_coder *coder)
{
    if (dongle->in_cooldown)
        {
            if (get_time() >= dongle->last_released + coder->sim->dongle_cooldown)
            {
                dongle->in_cooldown = 0; 
                return 1;
            }
            else
            {
                // use cond_timed_wait
                pthread_mutex_unlock(&dongle->mutex);
                usleep( (dongle->last_released + coder->sim->dongle_cooldown - get_time() )* 1000); 
                pthread_mutex_lock(&dongle->mutex);
                return 0; 
            }
        }
        return 1;
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

    if (!take_single_dongle(coder->first_dongle, coder))
        return (0);
        
    print_coder_status(coder, "has taken a dongle");

    if (!take_single_dongle(coder->second_dongle, coder))
        return (0);
    
    print_coder_status(coder, "has taken a dongle");

    return (1); 
}

void compile(s_coder *coder)
{
    if (take_dongles(coder))
    {
        if (coder->sim->simulation_ended)
        {
            release_dongles(coder->first_dongle, coder->second_dongle);
            return;
        }
        print_coder_status(coder, "is compiling");
        pthread_mutex_lock(&coder->lock);
        coder->last_compile_time = get_time();
        coder->compiles_done += 1;
        pthread_mutex_unlock(&coder->lock);
        usleep(coder->sim->time_to_compile * 1000);
        release_dongles(coder->first_dongle, coder->second_dongle);
    }
}
