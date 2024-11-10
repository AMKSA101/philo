/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 09:23:07 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/10 09:08:59 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PHILO_H__
# define __PHILO_H__

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct		s_data
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_of_philo;
	int				num_of_eat;
	int				philo_dead;
	long			start_time;
	long 			total_meals;
	long			finished_philos;
	pthread_mutex_t	death_check;
	pthread_mutex_t	meals_check;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
}					t_data;

typedef struct		s_philo
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

int	check_args(char **av);

#endif
