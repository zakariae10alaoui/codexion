#include "codexion.h"


void debuge(s_coder *coder)
{
    print_coder_status(coder,"is debugging");
    usleep((coder->sim->time_to_debug * 1000));
}