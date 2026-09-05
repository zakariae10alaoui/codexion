#include "codexion.h"

void *routine(void *arg)
{
    s_coder *coder;
    coder = (s_coder *)arg;

    while (1)
    {
        pthread_mutex_lock(&coder->sim->sim_lock);
        if (coder->sim->all_ready)
        {
            pthread_mutex_unlock(&coder->sim->sim_lock);
            break; 
        }
        pthread_mutex_unlock(&coder->sim->sim_lock);
        usleep(100); 
    }
    while (!check_sim(coder))
    {
        compile(coder);
        if (check_sim(coder))
            break;
        debuge(coder); 
        if (check_sim(coder))
            break;
        refactor(coder);   
        // if(coder->compiles_done < coder->sim->number_of_compiles_required)
        //     break;
    }
    return (NULL);
}

void simulation(s_sim *data)
{
    int i;
    pthread_t monitor;
    pthread_t tt[data->number_of_coders];

    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_create(&tt[i], NULL, routine, &data->coders[i]);
        i++;
    }
    init_start_time(data);
    pthread_create(&monitor, NULL, monitor_routine, data);
    pthread_join(monitor, NULL);
    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_join(tt[i], NULL);
        i++;
    }
}
