#include "codexion.h"

int take_single_dongle(s_dongle *dongle,int id)
{
    pthread_mutex_lock(&dongle->mutex);
    if (dongle->is_free || (dongle->in_cooldown))
    {
        return -1;
    }
    pthread_mutex_lock(&dongle->mutex);
}
int take_dongles(s_coder *coder)
{

}

void compile(s_coder *coder)
{
    take_dongles(coder);
    pthread_mutex_lock(&coder->sim->sim_lock);
    print_log(coder->sim, coder->id, "is compiling");
    coder->last_compile_start = get_time() + coder->sim->time_to_compile;
    coder->compiles_done += 1;
    pthread_mutex_unlock(&coder->sim->sim_lock);
    usleep(coder->sim->time_to_compile * 1000);
}
