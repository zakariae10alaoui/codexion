#include "codexion.h"

void refactor(s_coder *coder)
{
    coder->last_compile_start = get_time();
    pthread_mutex_lock(&coder->sim->print_lock);
    printf("coder %d is refactoring\n", coder->id);
    pthread_mutex_unlock(&coder->sim->print_lock);
    usleep((coder->sim->time_to_refactor * 1000));
}
