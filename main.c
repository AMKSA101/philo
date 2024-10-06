/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 09:23:02 by abamksa           #+#    #+#             */
/*   Updated: 2024/10/06 11:08:00 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;
	long			time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000;
	time += tv.tv_usec / 1000;
	return (time);
}

int		ft_atoi(const char *nptr)
{
	long	res;
	int		i;

	i = 0;
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

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print);
	if(!philo->data->philo_dead)
		printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->data->print);
}

void	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_check);
	if (get_time() - philo->last_eat > philo->data->time_to_die)
	{
		print_status(philo, "died");
		philo->data->philo_dead = 1;
	}
	pthread_mutex_unlock(&philo->data->death_check);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(100);
	while (!philo->data->philo_dead)
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_status(philo, "has taken a fork (left)");
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_status(philo, "has taken a fork (right)");
		print_status(philo, "is eating");
		philo->last_eat = get_time();
		ft_usleep(philo->data->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep * 1000);
		print_status(philo, "is thinking");
		check_death(philo);
	}
	return (NULL);
}


int		init_philo(t_data *data)
{
	int		i;
	t_philo	*philos;

	i = 0;
	philos = (t_philo *)malloc(sizeof(t_philo) * data->num_of_philo);
	if (!philos)
		return (-1);
	while (i < data->num_of_philo)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1) % data->num_of_philo;
		philos[i].last_eat = get_time();
		philos[i].data = data;
		i++;
	}
	i = 0;
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
	free(philos);
	return (0);
}


int		main(int ac, char **av)
{
	t_data	*data;
	int		i;

	i = 0;
	if (check_args(++av) == -1)
		return (1);
	if (ac < 5 || ac > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
	{
		printf("Error: Malloc failed\n");
		return (1);
	}
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
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_of_philo);
	if (!data->forks)
	{
		free(data);
		printf("Error: Malloc failed for forks\n");
		return (1);
	}
	while (i < data->num_of_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Error: Mutex initialization failed for fork %d\n", i);
			return (1);
		}
		i++;
	}
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death_check, NULL);
	if (init_philo(data) == -1)
	{
		printf("Error: Malloc failed\n");
		return (1);
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->death_check);
	return (0);
}
