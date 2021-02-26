#include "philosopher.h"
#include "util.h"

static int    takeFork(t_phil *phil)
{
    if (phil->my_number % 2)
    {
        pthread_mutex_lock(phil->left);
        if (get_usec() - phil->last_meal > phil->time_to_die)
        {
            pthread_mutex_unlock(phil->left);
            return (1);
        }
        print_status(get_usec(), phil->my_number, "has taken a fork\n");
		pthread_mutex_lock(phil->right);
    }
    else
    {
        pthread_mutex_lock(phil->right);
        if (get_usec() - phil->last_meal > phil->time_to_die)
        {
            pthread_mutex_unlock(phil->right);
            return (1);
        }
        print_status(get_usec(), phil->my_number, "has taken a fork\n");
        pthread_mutex_lock(phil->left);
    }
    return (0);
}

static void    dropFork(t_phil *phil)
{
    if (phil->my_number % 2)
    {
        pthread_mutex_unlock(phil->left);
		pthread_mutex_unlock(phil->right);
    }
    else
    {
        pthread_mutex_unlock(phil->right);
        pthread_mutex_unlock(phil->left);
    }
}

static int     eatNsleep(t_phil *phil)
{
    while (phil->must_eat)
    {
        if (takeFork(phil))
            return (0);
        if (get_usec() - phil->last_meal > phil->time_to_die)
        {
            dropFork(phil);
            return (0) ;
        }
        phil->last_meal = get_usec();
        print_status(get_usec(), phil->my_number, "is eating\n");
        usleep(phil->time_to_eat);
        dropFork(phil);
        if (get_usec() - phil->last_meal > phil->time_to_die)
            return (0);
        if (phil->must_eat > 0 && --(phil->must_eat) == 0)
            return (1);
        print_status(get_usec(), phil->my_number, "is sleeping\n");
        usleep(phil->time_to_sleep);
        if (get_usec() - phil->last_meal > phil->time_to_die)
            return (0);
        print_status(get_usec(), phil->my_number, "is thinking\n");
    }
    return (1);
}

static void    *waitMyDeath(void *info)
{
    t_phil  *phil;

    phil = (t_phil *)info;
    while (1)
    {
        if (get_usec() - phil->last_meal > phil->time_to_die)
        {
            if (phil->must_eat)
                print_status(get_usec(), phil->my_number, "died\n");
            break ;
        }
    }
    return (info);
}

void		*doPhilosopher(void *info)
{
	t_phil  *phil;
	pthread_t	death;

    phil = (t_phil *)info;
    phil->last_meal = get_usec();
	pthread_create(&death, NULL, waitMyDeath, info);
    eatNsleep(phil);
    pthread_join(death, NULL);
	return (info);
}