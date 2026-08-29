#include "codexion.h"

int check_burnout(s_sim *sim)
{
    int i;
    i = 0;
    while (i < sim->number_of_coders)
    {
        if (get_time() >= sim->coders[i].last_compile_start + sim->time_to_burnout)
        {
            printf("%d %d burned out",get_time(),sim->coders[i].id);
            return 0;
        }
        i++;
    }
}
int all_compile_done(s_sim *sim)
{
    int i;
    i = 0;
    while (i < sim->number_of_coders)
    {
        if (sim->coders[i].compiles_done == sim->number_of_compiles_required)
        {
            i++;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}
void *monitor_routine(s_sim *sim) 
{
    while (!sim->simulation_ended)
    {
        if (!check_burnout(sim))
        {
            sim->simulation_ended = 1;
            return NULL;
        }
        if(all_compile_done(sim))
        {
            sim->simulation_ended = 1;
            return NULL;
        }
        usleep(1000);
    }
}