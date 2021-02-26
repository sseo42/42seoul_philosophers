# include "util.h"

static struct timeval   g_current_time;

long int    get_usec(void)
{
    gettimeofday(&g_current_time, 0);
    return (g_current_time.tv_usec + g_current_time.tv_sec * 1000000);
}