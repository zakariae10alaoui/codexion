#include"codexion.h"

long long get_time()
{
    struct timeval tv;
    long long milsecond;

    gettimeofday(&tv , NULL);
    milsecond = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

    return milsecond;
}

void init_start_time(s_sim *data)
{
    int i;

    pthread_mutex_lock(&data->sim_lock);
    data->start_time = get_time();
    i = 0;
    while (i < data->number_of_coders)
    {
        data->coders[i].last_compile_start = data->start_time;
        i++;
    }
    data->simulation_ended = 0;
    data->all_ready = 1; 
    pthread_mutex_unlock(&data->sim_lock);
}
