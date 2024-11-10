/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 09:23:02 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/10 10:03:20 by abamksa          ###   ########.fr       */
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
			print_status(philo, "has taken a fork (left)");
			usleep(philo->data->time_to_die * 1000);
			print_status(philo, "died");
			pthread_mutex_lock(&philo->data->death_check);
			philo->data->philo_dead = 1;
			pthread_mutex_unlock(&philo->data->death_check);
			break ;
		}
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
		if (philo->data->num_of_eat != -1 && philo->eat_count >= philo->data->num_of_eat)
		{
			pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
			pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
			pthread_mutex_lock(&philo->data->meals_check);
			philo->data->finished_philos++;
			if (philo->data->finished_philos == philo->data->num_of_philo)
			{
				philo->data->total_meals = 1;
				pthread_mutex_unlock(&philo->data->meals_check);
			}
			else
				pthread_mutex_unlock(&philo->data->meals_check);
			break ;
		}
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_philo	*philos = (t_philo *)arg;
	t_data	*data = philos[0].data;
	int		i;

	while (1)
	{
		pthread_mutex_lock(&data->death_check);
		if (data->philo_dead || data->finished_philos >= data->num_of_philo)
		{
			pthread_mutex_unlock(&data->death_check);
			break ;
		}
		pthread_mutex_unlock(&data->death_check);

		i = 0;
		while (i < data->num_of_philo)
		{
			pthread_mutex_lock(&data->death_check);
			if (get_time() - philos[i].last_eat > data->time_to_die)
			{
				print_status(&philos[i], "died");
				data->philo_dead = 1;
				pthread_mutex_unlock(&data->death_check);
				return (NULL);
			}
			pthread_mutex_unlock(&data->death_check);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

int		init_philo(t_data *data)
{
	int		i;
	t_philo	*philos;
	pthread_t monitor_thread;

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
		philos[i].eat_count = 0;
		philos[i].data = data;
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, philos) != 0)
		return (-1);
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
	pthread_join(monitor_thread, NULL);
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
	data->total_meals = 0;
	data->finished_philos = 0;
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
	pthread_mutex_init(&data->meals_check, NULL);
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
	free(data->forks);
	free(data);
	return (0);
}
