#include "codexion.h"

int check_burnout(s_sim *sim)
{
    int i;
    i = 0;
    while (i < sim->number_of_coders)
    {
        pthread_mutex_lock(&sim->coders[i].lock);
        if (get_time() >= sim->coders[i].last_compile_time + sim->time_to_burnout)
        {
            printf("%lld %d burned out\n", get_time() - sim->start_time, sim->coders[i].id);
            pthread_mutex_unlock(&sim->coders[i].lock);
            return 0;
        }
        pthread_mutex_unlock(&sim->coders[i].lock);
        i++;
    }
    return 1;
}
int all_compile_done(s_sim *sim)
{
    int i;
    i = 0;
    while (i < sim->number_of_coders)
    {
        pthread_mutex_lock(&sim->coders[i].lock);
        // simulation ends after coder have compiled AT LEAST number_of_compiles_required times...
        if (sim->coders[i].compiles_done < sim->number_of_compiles_required)
        {
            pthread_mutex_unlock(&sim->coders[i].lock);
            return 0;
        }
        pthread_mutex_unlock(&sim->coders[i].lock);
        i++;
    }
    return 1;
}
void wake_em(s_sim *sim)
{
    int i;
    
     i = 0;
    while (i < sim->number_of_coders)
    {
        pthread_mutex_lock(&sim->dongles[i].mutex);
        pthread_cond_broadcast(&sim->dongles[i].cond);
        pthread_mutex_unlock(&sim->dongles[i].mutex);
        i++;
    }
}
void *monitor_routine(void *arg)
{
    s_sim *sim = (s_sim *)arg;
    while (!sim->simulation_ended)
    {
        if (!check_burnout(sim))
        {
            pthread_mutex_lock(&sim->sim_lock);
            sim->simulation_ended = 1;
            pthread_mutex_unlock(&sim->sim_lock);
            wake_em(sim);
            return NULL;
        }
        if(all_compile_done(sim))
        {
            pthread_mutex_lock(&sim->sim_lock);
            sim->simulation_ended = 1;
            pthread_mutex_unlock(&sim->sim_lock);
            wake_em(sim);
            return NULL;
        }
        usleep(500);
    }
    return NULL;
}

