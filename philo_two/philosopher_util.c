/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_util.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:56:28 by sseo              #+#    #+#             */
/*   Updated: 2021/03/01 00:21:27 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

extern sem_t	*g_peer_check;

static void		fillup_phil(t_arg arg, t_mtx *mtx, t_phil *phil)
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
		if (!idx)
			phil[idx].left_status = &(mtx[arg.number_of_phil - 1].status);
		else
			phil[idx].left_status = &(mtx[idx - 1].status);
		if (idx + 1 == arg.number_of_phil)
			phil[idx].right_status = &(mtx[0].status);
		else
			phil[idx].right_status = &(mtx[idx + 1].status);
		phil[idx].my_status = &(mtx[idx].status);
	}
}

int				launch_philosophers(t_arg arg, t_mtx *mtx, t_phil *phil)
{
	long int	idx;

	idx = 0;
	fillup_phil(arg, mtx, phil);
	while (idx < arg.number_of_phil)
	{
		if (pthread_create(&(mtx[idx].philosopher), NULL, do_philosopher, \
					(void *)(phil + idx)))
			return (3);
		idx++;
	}
	idx = 0;
	while (idx < arg.number_of_phil)
	{
		if (pthread_join(mtx[idx].philosopher, NULL))
			return (3);
		idx++;
	}
	free(phil);
	free(mtx);
	return (0);
}

void			check_peer(t_phil *phil)
{
	int			min_state;
	int			max_state;

	sem_wait(g_peer_check);
	max_state = (*(phil->left_status) > *(phil->right_status)) ? \
				*(phil->left_status) : *(phil->right_status);
	min_state = (*(phil->left_status) < *(phil->right_status)) ? \
				*(phil->left_status) : *(phil->right_status);
	if (*phil->my_status == SATISFIED)
	{
		if ((*(phil->left_status) == SATISFIED || \
				*(phil->left_status) == WAITING) && \
			(*(phil->right_status) == SATISFIED || \
				*(phil->right_status) == WAITING))
			*phil->my_status = EATING;
		else if (max_state == SATISFIED)
			*phil->my_status = min_state + 1;
	}
	else if (*phil->my_status > EATING)
	{
		if (*phil->my_status < max_state)
			*phil->my_status = EATING;
	}
	sem_post(g_peer_check);
}
