/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 09:23:07 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/11 12:11:43 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_philo;
	int				num_of_eat;
	int				philo_dead;
	long			start_time;
	long			total_meals;
	long			finished_philos;
	pthread_mutex_t	death_check;
	pthread_mutex_t	meals_check;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				eat_count;
	long			last_eat;
	pthread_t		thread;
	pthread_t		monitor_thread;
	t_data			*data;
}					t_philo;

long	get_time(void);
void	*monitor(void *arg);
void	*routine(void *arg);
void	ft_usleep(long time);
int		check_args(char **av);
int		check_args(char **av);
int		ft_atoi(const char *nptr);
int		start_philos(t_data *data);
void	cleanup_data(t_data *data);
void	eat_routine(t_philo *philo);
void	check_meals(t_philo *philo);
void	sleep_and_think(t_philo *philo);
void	single_philo_routine(t_philo *philo);
void	print_status(t_philo *philo, char *status);
int		init_data(int ac, char **av, t_data **data);
void	declare_data(int ac, char **av, t_data *data);

#endif
