/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:00:51 by sseo              #+#    #+#             */
/*   Updated: 2021/02/28 16:00:52 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "philosopher_func.h"

int				g_someone_dead;
pthread_mutex_t	g_death_check;
pthread_mutex_t	g_peer_check;

t_arg		extract_arg(char **argv, int option_flag)
{
	t_arg		arg;

	arg.number_of_phil = check_digit(argv[1]);
	arg.time_to_die = check_digit(argv[2]) * 1000;
	arg.time_to_eat = check_digit(argv[3]) * 1000;
	arg.time_to_sleep = check_digit(argv[4]) * 1000;
	if (arg.time_to_die < 0 || arg.time_to_eat < 0 || arg.time_to_sleep < 0)
		arg.number_of_phil = -1;
	if (option_flag)
	{
		arg.must_eat = check_digit(argv[5]);
		if (arg.must_eat < 0)
			arg.number_of_phil = -1;
	}
	else
		arg.must_eat = -1;
	return (arg);
}

t_mtx		*init_mtx(long int number_of_phil)
{
	t_mtx		*mtx;
	long int	idx;

	if (!(mtx = (t_mtx *)malloc(sizeof(t_mtx) * number_of_phil)))
		return (0);
	idx = 0;
	while (idx < number_of_phil)
	{
		pthread_mutex_init(&(mtx[idx].stick), NULL);
		mtx[idx].status = SATISFIED;
		idx++;
	}
	pthread_mutex_init(&g_death_check, NULL);
	pthread_mutex_init(&g_peer_check, NULL);
	return (mtx);
}

int			main(int argc, char **argv)
{
	t_arg		arg;
	t_mtx		*mtx;
	t_phil		*phil;

	g_someone_dead = 0;
	if (argc < 5 || argc > 6)
		return (1);
	arg = extract_arg(argv, argc == 6);
	if (arg.number_of_phil < 2)
		return (1);
	if (!(mtx = init_mtx(arg.number_of_phil)))
		return (2);
	if (!(phil = (t_phil *)malloc(sizeof(t_phil) * arg.number_of_phil)))
	{
		free(mtx);
		return (2);
	}
	if ((launch_philosophers(arg, mtx, phil)))
	{
		free(mtx);
		free(phil);
		return (3);
	}
	return (0);
}
