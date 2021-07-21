/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fminardi <fminardi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 17:40:50 by fminardi          #+#    #+#             */
/*   Updated: 2021/07/20 17:40:50 by fminardi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_time(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}

void	ft_usleep(int time)
{
	unsigned long	end;

	end = get_time() + time;
	while (get_time() < end)
		usleep(time);
}

void	ft_exit(t_table *table, t_philo **philo)
{
	int	i;
	int	num;

	num = table->num_phil;
	i = 0;
	i = 0;
	while (i < num)
	{
		free(table->fork[i]);
		i++;
	}
	free(table->fork);
	free(table);
	while (i < num)
	{
		free(philo[i]);
		i++;
	}
	free(philo);
}
