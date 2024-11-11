/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:55:12 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/11 12:14:14 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	print_status(philo, "has taken a fork (left)");
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	print_status(philo, "has taken a fork (right)");
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->death_check);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->data->death_check);
	ft_usleep(philo->data->time_to_eat);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
}

void	single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	print_status(philo, "has taken a fork (left)");
	usleep(philo->data->time_to_die * 1000);
	pthread_mutex_lock(&philo->data->meals_check);
	print_status(philo, "died");
	pthread_mutex_unlock(&philo->data->meals_check);
	pthread_mutex_lock(&philo->data->death_check);
	philo->data->philo_dead = 1;
	pthread_mutex_unlock(&philo->data->death_check);
}

void	check_meals(t_philo *philo)
{
	if (philo->data->num_of_eat != -1
		&& philo->eat_count >= philo->data->num_of_eat)
	{
		pthread_mutex_lock(&philo->data->meals_check);
		philo->data->finished_philos++;
		if (philo->data->finished_philos == philo->data->num_of_philo)
			philo->data->total_meals = 1;
		pthread_mutex_unlock(&philo->data->meals_check);
		pthread_exit(NULL);
	}
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
	print_status(philo, "is thinking");
}
