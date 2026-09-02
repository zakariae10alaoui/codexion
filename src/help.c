#include"codexion.h"

void fill_dongles(s_coder *coder, s_sim *sim) 
{
    int id = coder->id - 1;
    int num = sim->number_of_coders;

    if (coder->id % 2 == 0)
    {
        coder->first_dongle = &sim->dongles[id].mutex;
        coder->second_dongle = &sim->dongles[(id + 1) % num].mutex;
    }
    else
    {
        coder->first_dongle = &sim->dongles[(id + 1) % num].mutex;
        coder->second_dongle = &sim->dongles[id].mutex;
    }
}
