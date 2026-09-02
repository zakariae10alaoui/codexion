#include "codexion.h"

s_dongle *init_dongle(int nmb)
{
    int			i;
    s_dongle	*dongles;

    dongles = malloc(nmb * sizeof(s_dongle));
    if (dongles == NULL)
        return NULL;
    i = 0;
    while (i < nmb)
    {
        dongles[i].last_released = 0;
        dongles[i].is_free = 1;
        dongles[i].in_cooldown = 0;
        if (pthread_mutex_init(&dongles[i].mutex, NULL) != 0)
        {
            return NULL;
        }
        i++;
    }
    return dongles;
}

s_coder *init_coder(int nmb, s_sim *sim)
{
    int		i;
    s_coder	*coders;

    coders = malloc(nmb * sizeof(s_coder));
    if (coders == NULL)
        return NULL;
    i = 0;
    while (i < nmb)
    {
        coders[i].id = i + 1;
        fill_dongles(&coders[i],sim);
        coders[i].compiles_done = 0;
        coders[i].last_compile_start = 0;
        coders[i].sim = sim;
        i++;
    }
    return coders;
}

void fill_fields(s_sim *data, s_args *args)
{
    data->number_of_coders = args->number_of_coders;
    data->time_to_burnout = args->time_to_burnout;
    data->time_to_compile = args->time_to_compile;
    data->time_to_debug = args->time_to_debug;
    data->time_to_refactor = args->time_to_refactor;
    data->number_of_compiles_required = args->number_of_compiles_required;
    data->dongle_cooldown = args->dongle_cooldown;
    data->scheduler = args->scheduler;
    data->simulation_ended = 0;
    data->locks_ready = 0; 
}

int init_sim_resources(s_sim *data, s_args *args)
{
    data->dongles = init_dongle(args->number_of_coders);
    if (data->dongles == NULL)
        return -1;
    data->coders = init_coder(args->number_of_coders, data);
    if (data->coders == NULL)
        return -1;
    if (pthread_mutex_init(&data->print_lock, NULL) != 0)
        return -1;
    if (pthread_mutex_init(&data->sim_lock, NULL) != 0)
    {
        pthread_mutex_destroy(&data->print_lock);
        return -1;
    }
    data->locks_ready = 1;
    data->all_ready = 0;
    return 0;
}
 

s_sim *fill_it(s_args *args)
{
    s_sim *data;
 
    if (args->number_of_coders == 0)
    {
        printf("Number of coder can't be 0\n");
        free(args);
        return NULL;
    }
    data = malloc(sizeof(s_sim)); 
    if (data == NULL)
    {
      free(args);
      return NULL;
    }
    fill_fields(data, args);
    if (init_sim_resources(data, args) == -1)
    {
        freedom(data,args);
        return NULL;
    }
    return data;
}
