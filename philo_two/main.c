/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:36:57 by sseo              #+#    #+#             */
/*   Updated: 2021/03/01 01:12:47 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "philosopher_func.h"

int			g_someone_dead;
sem_t		*g_peer_check;
sem_t		*g_death_check;
sem_t		*g_sema;

t_arg			extract_arg(char **argv, int option_flag)
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

t_mtx			*init_mtx(long int number_of_phil)
{
	t_mtx		*mtx;
	long int	idx;

	g_someone_dead = 0;
	sem_unlink(".Philosopher");
	sem_unlink(".Philosopher_dead");
	sem_unlink(".Philosopher_peer");
	if ((g_sema = sem_open(".Philosopher", O_CREAT, 0644, number_of_phil)) \
			== SEM_FAILED)
		return (0);
	if ((g_death_check = sem_open(".Philosopher_dead", O_CREAT, 0644, 1)) \
			== SEM_FAILED)
		return (0);
	if ((g_peer_check = sem_open(".Philosopher_peer", O_CREAT, 0644, 1)) \
			== SEM_FAILED)
		return (0);
	if (!(mtx = (t_mtx *)malloc(sizeof(t_mtx) * number_of_phil)))
		return (0);
	idx = 0;
	while (idx < number_of_phil)
	{
		mtx[idx].status = SATISFIED;
		idx++;
	}
	return (mtx);
}

int				main(int argc, char **argv)
{
	t_arg		arg;
	t_mtx		*mtx;
	t_phil		*phil;

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
	sem_close(g_sema);
	sem_close(g_death_check);
	sem_close(g_peer_check);
}
