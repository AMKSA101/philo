/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:47:29 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/11 12:00:02 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	declare_data(int ac, char **av, t_data *data)
{
	data->num_of_philo = ft_atoi(av[0]);
	data->time_to_die = ft_atoi(av[1]);
	data->time_to_eat = ft_atoi(av[2]);
	data->time_to_sleep = ft_atoi(av[3]);
	if (ac == 6)
		data->num_of_eat = ft_atoi(av[4]);
	else
		data->num_of_eat = -1;
	data->start_time = get_time();
	data->philo_dead = 0;
	data->total_meals = 0;
	data->finished_philos = 0;
}

int	init_data(int ac, char **av, t_data **data)
{
	int	i;

	i = -1;
	*data = (t_data *)malloc(sizeof(t_data));
	if (!*data)
		return (printf("Error: Malloc failed\n"), 1);
	declare_data(ac, av, *data);
	(*data)->forks = malloc(sizeof(pthread_mutex_t) * (*data)->num_of_philo);
	if (!(*data)->forks)
	{
		free(*data);
		return (printf("Error: Malloc failed for forks\n"), 1);
	}
	while (++i < (*data)->num_of_philo)
	{
		if (pthread_mutex_init(&(*data)->forks[i], NULL) != 0)
			return (printf("Error: Mutex initialization failed for fork %d\n",
					i), 1);
	}
	pthread_mutex_init(&(*data)->print, NULL);
	pthread_mutex_init(&(*data)->death_check, NULL);
	pthread_mutex_init(&(*data)->meals_check, NULL);
	if (start_philos(*data) == -1)
		return (printf("Error: Malloc failed\n"), 1);
	return (0);
}

void	cleanup_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_of_philo)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->death_check);
	pthread_mutex_destroy(&data->meals_check);
	free(data->forks);
	free(data);
}
