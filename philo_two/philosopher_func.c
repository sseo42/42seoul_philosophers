/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_func.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:52:24 by sseo              #+#    #+#             */
/*   Updated: 2021/03/01 01:12:54 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_func.h"

static struct timeval	g_current_time;

long int		get_usec(void)
{
	gettimeofday(&g_current_time, 0);
	return (g_current_time.tv_usec + g_current_time.tv_sec * 1000000);
}

void			print_status(long int usec, long int number, char *msg)
{
	char		buf[100];
	long int	len;
	int			idx;

	len = LONG_MAX_DIGIT;
	idx = 0;
	while (len / 1000)
	{
		buf[idx] = (usec / len) % 10 + '0';
		idx++;
		len /= 10;
	}
	buf[idx] = ' ';
	len = 1;
	while (number >= len)
		len *= 10;
	len /= 10;
	while (len)
	{
		buf[++idx] = (number / len) % 10 + '0';
		len /= 10;
	}
	buf[idx + 1] = ' ';
	len = ft_str_insert(buf, msg, idx + 2);
	write(1, buf, len);
}

int				ft_str_insert(char *buf, char *msg, int start_idx)
{
	int			i;

	i = 0;
	while (msg[i])
	{
		buf[start_idx + i] = msg[i];
		i++;
	}
	buf[start_idx + i] = 0;
	return (start_idx + i);
}

long int		check_digit(char *str)
{
	long int	idx;
	long int	val;

	idx = 0;
	val = 0;
	while (str[idx])
	{
		if (str[idx] >= '0' && str[idx] <= '9')
		{
			val = val * 10 + str[idx] - '0';
			if (val >= 10000000000000000)
				return (-1);
		}
		else
			return (-1);
		idx++;
	}
	return (val);
}
