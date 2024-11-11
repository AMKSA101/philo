/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:08:48 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/11 12:15:10 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long time)
{
	struct timeval	current;
	struct timeval	start;
	int				start_time;
	int				current_time;
	long			sleep_time;

	sleep_time = 1;
	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		start_time = (start.tv_sec * 1000) + (start.tv_usec / 1000);
		current_time = (current.tv_sec * 1000) + (current.tv_usec / 1000);
		if (current_time - start_time >= time)
			break ;
		usleep(sleep_time);
	}
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print);
	if (!philo->data->philo_dead)
		printf("%ld %d %s\n", get_time() - philo->data->start_time,
			philo->id, status);
	pthread_mutex_unlock(&philo->data->print);
}

long	get_time(void)
{
	struct timeval	tv;
	long			time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000;
	time += tv.tv_usec / 1000;
	return (time);
}

int	ft_atoi(const char *nptr)
{
	long	res;

	res = 0;
	while ((*nptr >= '\t' && *nptr <= '\r') || (*nptr == ' '))
	{
		++nptr;
		if (*nptr == '\0')
			return (-1);
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + (*nptr - 48);
		if (res > 2147483647 || !res)
			return (-1);
		++nptr;
	}
	while ((*nptr >= '\t' && *nptr <= '\r') || (*nptr == ' '))
		++nptr;
	if ((*nptr > 31 && *nptr < 127))
		return (-1);
	return (res);
}
