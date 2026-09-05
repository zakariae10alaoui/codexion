#include "codexion.h"
#include <pthread.h>

void free_dongles(s_dongle *dongles, int nmb)
{
    int i;

    i = 0;
    while (i < nmb)
    {
        pthread_mutex_destroy(&dongles[i].mutex);
        pthread_cond_destroy(&dongles[i].cond);
        i++;
    }
    free(dongles);
}


void freedom(s_sim *sim, s_args *args)
{

    free(sim->coders);
    free_dongles(sim->dongles, sim->number_of_coders);
    if (sim->locks_ready) 
    {
        pthread_mutex_destroy(&sim->print_lock);
        pthread_mutex_destroy(&sim->sim_lock);
    }
    free(sim);
    free(args); 
}
