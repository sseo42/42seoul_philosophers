
#include "util.h"
#include "stdio.h"
/*
**  add msg to buf
**  return length of the result str
*/

int     ft_str_insert(char *buf, char *msg, int start_idx)
{
    int         i;

    i = 0;
    while (msg[i])
    {
        buf[start_idx + i] = msg[i];
        i++;
    }
    buf[start_idx + i] = 0;
    return (start_idx + i);
}

long int    check_digit(char *str)
{
    long int      idx;
    long int      val;

    idx = 0;
    val = 0;
    while (str[idx])
    {
        if (str[idx] >= '0' && str[idx] <= '9')
        {
            val = val * 10 + str[idx] - '0';
            if (val >= 10000000000000000)
                return (-1);
        }
        else
            return (-1);
        idx++;
    }
    return (val);
}