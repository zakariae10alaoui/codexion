#include"codexion.h"

void fill_dongles(s_coder *coder, s_sim *sim) 
{
    int id = coder->id - 1;
    int num = sim->number_of_coders;

    if (coder->id % 2 == 0)
    {
        coder->first_dongle = &sim->dongles[id];
        coder->second_dongle = &sim->dongles[(id + 1) % num];
    }
    else
    {
        coder->first_dongle = &sim->dongles[(id + 1) % num];
        coder->second_dongle = &sim->dongles[id];
    }
}

int check_sim(s_coder *coder)
{
    int ended;

    pthread_mutex_lock(&coder->sim->sim_lock);
    ended = coder->sim->simulation_ended;
    pthread_mutex_unlock(&coder->sim->sim_lock);
    return ended;
}
void print_coder_status(s_coder *coder,char *status)
{
    pthread_mutex_lock(&coder->sim->print_lock);
    
    if (!coder->sim->simulation_ended)
    {
        long long timestamp = get_time() - coder->sim->start_time;
        printf("%lld %d %s\n", timestamp, coder->id, status);
    }
    
    pthread_mutex_unlock(&coder->sim->print_lock);
}
void release_dongles(s_dongle *d1, s_dongle *d2)
{
    pthread_mutex_lock(&d1->mutex);
    d1->is_free = 1;
    d1->in_cooldown = 1;
    d1->last_released = get_time();
    
    pthread_cond_broadcast(&d1->cond); 
    pthread_mutex_unlock(&d1->mutex);

    pthread_mutex_lock(&d2->mutex);
    d2->is_free = 1;
    d2->in_cooldown = 1;
    d2->last_released = get_time();
    
    pthread_cond_broadcast(&d2->cond);
    pthread_mutex_unlock(&d2->mutex);
}
