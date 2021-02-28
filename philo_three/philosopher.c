/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 00:15:24 by sseo              #+#    #+#             */
/*   Updated: 2021/03/01 01:22:27 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "philosopher_func.h"

extern sem_t	*g_death_check;
extern sem_t	*g_peer_check;
extern sem_t	*g_sema;
extern sem_t	*g_last_word;

static int		take_stick(t_phil *phil)
{
	sem_wait(g_peer_check);
	sem_wait(g_sema);
	if (get_usec() - phil->last_meal > phil->time_to_die)
	{
		sem_post(g_sema);
		sem_post(g_peer_check);
		return (1);
	}
	print_status(get_usec(), phil->my_number, "has taken a Fork\n");
	sem_wait(g_sema);
	sem_post(g_peer_check);
	return (0);
}

static int		drop_stick(void)
{
	sem_post(g_sema);
	sem_post(g_sema);
	return (0);
}

static int		eat_sleep(t_phil *phil)
{
	while (phil->must_eat)
	{
		if (get_usec() - phil->last_meal > phil->time_to_die)
			return (0);
		if (take_stick(phil))
			return (0);
		if (get_usec() - phil->last_meal > phil->time_to_die)
			return (drop_stick());
		phil->last_meal = get_usec();
		print_status(get_usec(), phil->my_number, "is eating\n");
		usleep(phil->time_to_eat);
		drop_stick();
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

static void		*wait_my_death(void *info)
{
	t_phil		*phil;

	phil = (t_phil *)info;
	while (1)
	{
		sem_wait(g_death_check);
		if (get_usec() - phil->last_meal > phil->time_to_die)
		{
			if (phil->must_eat)
			{
				print_status(get_usec(), phil->my_number, "died\n");
			}
			sem_post(g_last_word);
			usleep(500);
			sem_post(g_death_check);
			break ;
		}
		sem_post(g_death_check);
	}
	return (info);
}

void			*do_philosopher(void *info)
{
	t_phil		*phil;
	pthread_t	death;
	pthread_t	last;

	if ((g_sema = sem_open(".Pl", 0)) == SEM_FAILED)
		return (0);
	if ((g_death_check = sem_open(".Pl_dead", 0)) == SEM_FAILED)
		return (0);
	if ((g_peer_check = sem_open(".Pl_peer", 0)) == SEM_FAILED)
		return (0);
	if ((g_last_word = sem_open(".Pl_last", 0)) == SEM_FAILED)
		return (0);
	phil = (t_phil *)info;
	phil->last_meal = get_usec();
	pthread_create(&death, NULL, wait_my_death, info);
	pthread_create(&last, NULL, check_last, info);
	eat_sleep(phil);
	pthread_join(death, NULL);
	pthread_join(last, NULL);
	return (info);
}
