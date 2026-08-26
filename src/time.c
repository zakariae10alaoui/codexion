#include"codexion.h"

long long get_time()
{
    struct timeval tv;
    long long milsecond;

    gettimeofday(&tv , NULL);
    milsecond = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

    return milsecond;
}