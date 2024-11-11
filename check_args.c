/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 09:22:04 by abamksa           #+#    #+#             */
/*   Updated: 2024/11/11 11:58:45 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(void)
{
	write(2, "Invalid arg: The arg should be a positive integer!", 50);
}

int	scanit(const char *nptr)
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
		{
			print_error();
			return (-1);
		}
		++nptr;
	}
	while ((*nptr >= '\t' && *nptr <= '\r') || (*nptr == ' '))
		++nptr;
	if ((*nptr > 31 && *nptr < 127))
		return (print_error(), -1);
	return (res);
}

int	check_args(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (!av[i])
		{
			print_error();
			return (-1);
		}
		if (scanit(av[i]) == -1)
		{
			print_error();
			return (-1);
		}
		i++;
	}
	return (0);
}
