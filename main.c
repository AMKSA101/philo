/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 09:23:02 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/11 12:08:53 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_check);
		if (philo->data->philo_dead)
		{
			pthread_mutex_unlock(&philo->data->death_check);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_check);
		if (philo->data->num_of_philo == 1)
		{
			single_philo_routine(philo);
			break ;
		}
		eat_routine(philo);
		check_meals(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}

int	check_status(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_lock(&data->death_check);
		if (get_time() - philos[i].last_eat > data->time_to_die)
		{
			pthread_mutex_lock(&data->meals_check);
			print_status(&philos[i], "died");
			data->philo_dead = 1;
			pthread_mutex_unlock(&data->meals_check);
			pthread_mutex_unlock(&data->death_check);
			return (1);
		}
		pthread_mutex_unlock(&data->death_check);
		i++;
	}
	return (0);
}

void	lock_mutex(pthread_mutex_t *death_check, pthread_mutex_t *meals_check)
{
	pthread_mutex_lock(death_check);
	pthread_mutex_lock(meals_check);
}

void	*monitor(void *arg)
{
	t_philo	*philos;
	t_data	*data;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (1)
	{
		lock_mutex(&data->death_check, &data->meals_check);
		if (data->philo_dead || data->finished_philos >= data->num_of_philo)
		{
			pthread_mutex_unlock(&data->death_check);
			break ;
		}
		pthread_mutex_unlock(&data->death_check);
		if (data->total_meals)
		{
			pthread_mutex_unlock(&data->meals_check);
			break ;
		}
		pthread_mutex_unlock(&data->meals_check);
		if (check_status(philos, data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (check_args(++av) == -1)
		return (1);
	if (ac < 5 || ac > 6)
		return (printf("Error: Wrong number of arguments\n"), 1);
	if (init_data(ac, av, &data) != 0)
		return (1);
	cleanup_data(data);
	return (0);
}
