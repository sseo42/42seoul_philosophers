#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_arg
{
    long int            number_of_phil;
    long int            time_to_die;
    long int            time_to_eat;
    long int            time_to_sleep;
    long int            must_eat;
}   t_arg;

typedef struct s_mtx
{
    pthread_t           philosopher;
    pthread_mutex_t     stick;
}   t_mtx;

typedef struct  s_phil
{
    long int            my_number;
    long int            time_to_die;
    long int            time_to_eat;
    long int            time_to_sleep;
    long int            must_eat;
    pthread_mutex_t     *left;
    pthread_mutex_t     *right;
    long int            last_meal;
}       t_phil;

void		*doPhilosopher(void *info);

#endif