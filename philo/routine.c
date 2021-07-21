/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fminardi <fminardi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 17:40:33 by fminardi          #+#    #+#             */
/*   Updated: 2021/07/20 17:40:33 by fminardi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	sleeping(t_philo *p)
{
	p->status = SLEEP;
	pthread_mutex_lock(&p->table->print);
	if (p->table->kill)
	{
		pthread_mutex_unlock(&p->table->print);
		return (0);
	}
	printf("[%lli] %i \033[0;35mis sleeping\033[0;37m\n", get_time()
		- p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	ft_usleep(p->table->ts);
	pthread_mutex_lock(&p->table->print);
	if (p->table->kill)
	{
		pthread_mutex_unlock(&p->table->print);
		return (0);
	}
	printf("[%lli] %i \033[0;36mis thinking\033[0;37m\n", get_time()
		- p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	if (p->table->kill)
		return (0);
	return (1);
}

int	eating(t_philo *p)
{
	p->status = EAT;
	p->count++;
	pthread_mutex_lock(&p->table->print);
	if (p->table->kill)
	{
		pthread_mutex_unlock(p->table->fork[p->i - 1]);
		pthread_mutex_unlock(p->table->fork[p->i]);
		pthread_mutex_unlock(&p->table->print);
		return (0);
	}
	printf("[%lli] %i \e[4;34m\e[1;97mis eating\033[0;37m\n", get_time()
		- p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	p->start_eat = get_time();
	ft_usleep(p->table->te);
	pthread_mutex_unlock(p->table->fork[p->i - 1]);
	pthread_mutex_unlock(p->table->fork[p->i]);
	return (1);
}

int	take_right_fork(t_philo *p)
{
	pthread_mutex_lock(p->table->fork[p->i]);
	pthread_mutex_lock(&p->table->print);
	if (p->table->kill)
	{
		pthread_mutex_unlock(p->table->fork[p->i - 1]);
		pthread_mutex_unlock(&p->table->print);
		return (0);
	}
	printf("[%lli] %i \033[0;34mhas taken a fork\033[0;37m\n", get_time()
		- p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	return (1);
}

int	take_left_fork(t_philo *p)
{
	pthread_mutex_lock(p->table->fork[p->i - 1]);
	pthread_mutex_lock(&p->table->print);
	if (p->table->kill)
	{
		pthread_mutex_unlock(p->table->fork[p->i - 1]);
		pthread_mutex_unlock(&p->table->print);
		return (0);
	}
	printf("[%lli] %i \033[0;34mhas taken a fork\033[0;37m\n", get_time()
		- p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	return (1);
}

void	*routine(void *philo)
{
	t_philo		*p;
	pthread_t	died;

	p = (t_philo *)philo;
	pthread_create(&died, NULL, &check_death, (void *)philo);
	if (p->i % 2 == 0)
		ft_usleep(p->table->te);
	while (!p->table->kill)
	{
		if (p->table->kill)
			break ;
		if (!take_left_fork(p))
			return (0);
		if (!take_right_fork(p))
			return (0);
		if (!eating(p))
			return (0);
		if (!sleeping(p))
			return (0);
	}
	pthread_join(died, NULL);
	return (NULL);
}
