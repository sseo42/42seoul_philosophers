/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_func.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sseo <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 00:15:09 by sseo              #+#    #+#             */
/*   Updated: 2021/03/01 00:15:10 by sseo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_FUNC_H
# define PHILOSOPHER_FUNC_H

# include <unistd.h>
# include <sys/time.h>

# define LONG_MAX_DIGIT 1000000000000000000

long int	get_usec(void);
int			ft_str_insert(char *buf, char *msg, int start_idx);
void		print_status(long int usec, long int number, char *msg);
long int	check_digit(char *str);

#endif
