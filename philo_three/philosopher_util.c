/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 00:08:38 by sseo              #+#    #+#             */
/*   Updated: 2021/03/01 01:23:09 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "philosopher_func.h"

extern sem_t	*g_last_word;

static void		fillup_phil(t_arg arg, t_phil *phil)
{
	long int	idx;

	idx = -1;
	while (++idx < arg.number_of_phil)
	{
		phil[idx].my_number = idx + 1;
		phil[idx].time_to_die = arg.time_to_die;
		phil[idx].time_to_eat = arg.time_to_eat;
		phil[idx].time_to_sleep = arg.time_to_sleep;
		phil[idx].must_eat = arg.must_eat;
	}
}

int				launch_philosophers(t_arg arg, t_mtx *mtx, t_phil *phil)
{
	long int	idx;
	int			status;
	pid_t		pid_num;

	idx = 0;
	fillup_phil(arg, phil);
	while (idx < arg.number_of_phil)
	{
		if ((pid_num = fork()) == 0)
		{
			do_philosopher((void *)(&(phil[idx])));
			free(mtx);
			free(phil);
			exit(0);
		}
		idx++;
	}
	idx = 0;
	while (idx < arg.number_of_phil)
	{
		waitpid(0, &status, 0);
		idx++;
	}
	return (0);
}

void			*check_last(void *info)
{
	t_phil		*phil;

	phil = (t_phil *)info;
	sem_wait(g_last_word);
	phil->last_meal = get_usec() - phil->time_to_die - 1;
	phil->must_eat = 0;
	sem_post(g_last_word);
	return (info);
}
