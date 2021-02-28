/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:47:13 by sseo              #+#    #+#             */
/*   Updated: 2021/03/01 01:12:39 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "philosopher_func.h"

extern int		g_someone_dead;
extern sem_t	*g_death_check;
extern sem_t	*g_sema;

static int		take_stick(t_phil *phil)
{
	if (get_usec() - phil->last_meal > phil->time_to_die)
		return (1);
	while (*(phil->my_status) != EATING)
	{
		usleep(50);
		check_peer(phil);
	}
	sem_wait(g_sema);
	if (get_usec() - phil->last_meal > phil->time_to_die)
	{
		*(phil->my_status) = SATISFIED;
		sem_post(g_sema);
		return (1);
	}
	print_status(get_usec(), phil->my_number, "has taken a Fork\n");
	sem_wait(g_sema);
	return (0);
}

static int		drop_stick(t_phil *phil)
{
	sem_post(g_sema);
	sem_post(g_sema);
	*(phil->my_status) = SATISFIED;
	return (0);
}

static int		eat_sleep(t_phil *phil)
{
	while (phil->must_eat && !g_someone_dead)
	{
		if (take_stick(phil))
			return (0);
		if (get_usec() - phil->last_meal > phil->time_to_die)
			return (drop_stick(phil));
		phil->last_meal = get_usec();
		print_status(get_usec(), phil->my_number, "is eating\n");
		usleep(phil->time_to_eat);
		drop_stick(phil);
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
		if (g_someone_dead)
			phil->last_meal = get_usec() - phil->time_to_die - 1;
		if (get_usec() - phil->last_meal > phil->time_to_die)
		{
			if (phil->must_eat && !g_someone_dead)
				print_status(get_usec(), phil->my_number, "died\n");
			sem_post(g_death_check);
			g_someone_dead = 1;
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

	phil = (t_phil *)info;
	phil->last_meal = get_usec();
	pthread_create(&death, NULL, wait_my_death, info);
	eat_sleep(phil);
	pthread_join(death, NULL);
	return (info);
}
