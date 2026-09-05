#include "codexion.h"

void pop_from_queue(s_dongle *dongle)
{
    dongle->nodes[0] = dongle->nodes[1];
    
    dongle->nodes[1].coder_id = 0;
    dongle->nodes[1].priorrity = 0;
    dongle->nodes[1].size = 0;
}



long long choose_priority(s_coder *coder)
{
    long long priority;

    // pthread_mutex_lock(&coder->sim->sim_lock);
    
    if (strcmp(coder->sim->scheduler, "edf") == 0)
    {
        priority = coder->last_compile_time + coder->sim->time_to_burnout;
    }
    else
    {
        priority = get_time();
    }
    
    // pthread_mutex_unlock(&coder->sim->sim_lock);
    return priority;
}
void swap_coder(s_dongle *dongle)
{
    s_queue temp;

    if (dongle->nodes[1].size == 0)
        return ;

    if (dongle->nodes[0].size == 1 && dongle->nodes[1].size == 1)
    {
        if (dongle->nodes[1].priorrity < dongle->nodes[0].priorrity)
        {
            temp = dongle->nodes[0];
            dongle->nodes[0] = dongle->nodes[1];
            dongle->nodes[1] = temp;
        }
    }
}
void register_in_queue(s_dongle *dongle, s_coder *coder)
{
    if (dongle->nodes[0].size == 0)
    {
        dongle->nodes[0].coder_id = coder->id;
        dongle->nodes[0].priorrity = choose_priority(coder);
        dongle->nodes[0].size = 1;
    }
    else {
        dongle->nodes[1].coder_id = coder->id;
        dongle->nodes[1].priorrity = choose_priority(coder);
        dongle->nodes[1].size = 1;
    }
    swap_coder(dongle);
}

