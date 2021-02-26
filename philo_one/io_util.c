#include "util.h"

void    print_status(long int usec, long int number, char *msg)
{
    char        buf[100];
    long int    len;
    int         idx;

    len = LONG_MAX_DIGIT;
    idx = 0;
    while (len / 1000)
    {
        buf[idx] = (usec / len) % 10 + '0';
        idx++;
        len /= 10;
    }
    buf[idx] = ' ';
    len = 1;
    while (number >= len)
        len *= 10;
    len /= 10;
    while (len)
    {
        buf[++idx] = (number / len) % 10 + '0';
        len /= 10;
    }
    buf[idx + 1] = ' ';
    len = ft_str_insert(buf, msg, idx + 2);
    write(1, buf, len);
}