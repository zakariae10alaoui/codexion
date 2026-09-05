#include "codexion.h"

void refactor(s_coder *coder)
{
    print_coder_status(coder,"is refactoring");
    usleep((coder->sim->time_to_refactor * 1000));
}
