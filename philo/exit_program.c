/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fminardi <fminardi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 17:40:38 by fminardi          #+#    #+#             */
/*   Updated: 2021/07/20 17:40:38 by fminardi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_eaten(t_philo *philo)
{	
	if (philo->count == philo->table->max_eat && !philo->btdt)
	{
		philo->table->finish++;
		philo->btdt = 1;
	}
	if (philo->table->finish == philo->table->num_phil)
	{
		pthread_mutex_lock(&philo->table->killed);
		philo->table->kill = 1;
		pthread_mutex_unlock(&philo->table->killed);
		return (0);
	}
	return (1);
}

void	*check_death(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	while (1)
	{
		if (philo->table->max_eat)
			if (!check_eaten(philo))
				return (NULL);
		if ((get_time() - philo->start_eat) > philo->table->td)
		{
			pthread_mutex_lock(&philo->table->killed);
			philo->status = DIE;
			pthread_mutex_lock(&philo->table->print);
			if (!philo->table->kill)
				printf("\033[0;31m[%lli] %i died\033[0;37m\n", get_time()
					- philo->table->start_prog, philo->i);
			pthread_mutex_unlock(philo->table->fork[0]);
			philo->table->kill = 1;
			pthread_mutex_unlock(&philo->table->print);
			pthread_mutex_unlock(&philo->table->killed);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
