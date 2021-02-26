#ifndef UTIL_H
# define UTIL_L

# include <unistd.h>
# include <sys/time.h>

# define LONG_MAX_DIGIT 1000000000000000000

long int    get_usec(void);
int     ft_str_insert(char *buf, char *msg, int start_idx);
void    print_status(long int usec, long int number, char *msg);
long int    check_digit(char *str);

#endif