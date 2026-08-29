#include "codexion.h"
void take_single_dongle(s_dongle *dongle,int id)
{
    pthread_mutex_lock(&dongle->mutex);
    if (!(dongle->is_free) || (dongle->in_cooldown))
    {
        
    }
    
    printf("%d %d has taken a dongle\n",get_time(),id);
    pthread_mutex_lock(&dongle->mutex);
}
int take_dongles(s_coder *coder)
{
    if ((coder->id % 2) != 0);
        usleep(200);
    
    take_single_dongle(coder->left_dongle ,coder->id);
    take_single_dongle(coder->right_dongle, coder->id);
    
}

void compile(s_coder *coder)
{
    take_dongles(coder);

    pthread_mutex_lock(&coder->sim->sim_lock);
    coder->last_compile_start = get_time();
    print_log(coder->sim, coder->id, "is compiling");
    pthread_mutex_unlock(&coder->sim->sim_lock);

    usleep(coder->sim->time_to_compile * 1000);
}
