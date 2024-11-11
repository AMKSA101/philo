/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:59:28 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/11 12:13:20 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos(t_data *data, t_philo **philos)
{
	int	i;

	i = 0;
	*philos = (t_philo *)malloc(sizeof(t_philo) * data->num_of_philo);
	if (!*philos)
		return (-1);
	while (i < data->num_of_philo)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].left_fork = i;
		(*philos)[i].right_fork = (i + 1) % data->num_of_philo;
		(*philos)[i].last_eat = get_time();
		(*philos)[i].eat_count = 0;
		(*philos)[i].data = data;
		i++;
	}
	return (0);
}

int	create_join(t_data *data, t_philo *philos)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	if (pthread_create(&monitor_thread, NULL, monitor, philos) != 0)
		return (-1);
	while (i < data->num_of_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, routine, &philos[i]) != 0)
			return (-1);
		i++;
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (-1);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	return (0);
}

int	start_philos(t_data *data)
{
	t_philo	*philos;

	if (init_philos(data, &philos) != 0)
		return (-1);
	if (create_join(data, philos) != 0)
		return (-1);
	free(philos);
	return (0);
}
