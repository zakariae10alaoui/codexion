#include "codexion.h"
#include <pthread.h>
#include <unistd.h>

void *routine(void * coder)
{
    compile((s_coder *)&coder);
    debuge((s_coder *)coder);
    refactor((s_coder *)coder);

}

void simulation(s_sim *data)
{
    int i;
    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_t tt;
        data->coders[i].thread = tt;
        pthread_create(&tt, NULL,routine,&data->coders[i]);
        i++;
    }
}
