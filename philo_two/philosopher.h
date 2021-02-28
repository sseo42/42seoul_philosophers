/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:41:25 by sseo              #+#    #+#             */
/*   Updated: 2021/02/28 16:42:15 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>

# define SATISFIED	2147483646
# define EATING		0
# define WAITING	1

typedef struct	s_arg
{
	long int	number_of_phil;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
	long int	must_eat;
}				t_arg;

typedef struct	s_mtx
{
	pthread_t	philosopher;
	int			status;
}				t_mtx;

typedef struct	s_phil
{
	long int	my_number;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
	long int	must_eat;
	long int	last_meal;
	int			*left_status;
	int			*my_status;
	int			*right_status;
}				t_phil;

void			check_peer(t_phil *phil);
void			*do_philosopher(void *info);
int				launch_philosophers(t_arg arg, t_mtx *mtx, t_phil *phil);

#endif
