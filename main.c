/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 09:23:02 by abamksa           #+#    #+#             */
/*   Updated: 2024/09/29 10:40:37 by abamksa          ###   ########.fr       */
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
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&philo->data->forks[(philo->id) % philo->data->num_of_philo]);
			print_status(philo, "has taken a fork (right)");
			pthread_mutex_lock(&philo->data->forks[philo->id - 1]);
			print_status(philo, "has taken a fork (left)");
		}
		else
		{
			pthread_mutex_lock(&philo->data->forks[philo->id - 1]);
			print_status(philo, "has taken a fork (left)");
			pthread_mutex_lock(&philo->data->forks[(philo->id) % philo->data->num_of_philo]);
			print_status(philo, "has taken a fork (right)");
		}
		print_status(philo, "is eating");
		philo->last_eat = get_time();
		usleep(philo->data->time_to_eat * 1000);
		pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
		pthread_mutex_unlock(&philo->data->forks[(philo->id) % philo->data->num_of_philo]);
		print_status(philo, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
		print_status(philo, "is thinking");
		if (philo->data->num_of_eat != -1)
		{
			philo->data->num_of_eat++;
			if (philo->data->num_of_eat == philo->data->num_of_philo)
				break;
		}
	}
	return (NULL);
}


int		init_philo(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = (t_philo *)malloc(sizeof(t_philo) * data->num_of_philo);
	if (!philo)
		return (-1);
	while (i < data->num_of_philo)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].last_eat = get_time();
		philo[i].data->num_of_eat = 0;
		i++;
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]) != 0)
			return (-1);
		i++;
	}
	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (-1);
		i++;
	}
	free(philo);
	return (0);
}


int		main(int ac, char **av)
{
	t_data	*data;
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
	if (init_philo(data) == -1)
	{
		printf("Error: Malloc failed\n");
		return (1);
	}
	return (0);
}
