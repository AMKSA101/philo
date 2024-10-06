/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 11:04:05 by abamksa           #+#    #+#             */
/*   Updated: 2024/10/06 11:06:49 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	exit_error(char *str)
{
	printf("%s", str);
	exit(1);
}

void	ft_usleep(long time)
{
	long	start;

	start = get_time();
	while (get_time() < start + time)
		usleep(100);
}
