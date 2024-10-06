/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abamksa <abamksa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 10:05:12 by abamksa           #+#    #+#             */
/*   Updated: 2024/10/06 11:03:57 by abamksa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		exit_error("Error: Malloc failed\n");
	return (ptr);
}

void	*safe_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = calloc(count, size);
	if (!ptr)
	{
		printf("Error: Calloc failed\n");
		exit(1);
	}
	return (ptr);
}

void	safe_free(void *ptr)
{
	if (ptr)
		free(ptr);
}

