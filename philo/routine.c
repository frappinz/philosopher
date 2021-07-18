#include "philo.h"

int	sleeping(t_philo *p)
{
	p->status = SLEEP;
	if (p->table->kill)
	{
		pthread_mutex_unlock(&p->table->killed);
		return (0);
	}
	pthread_mutex_lock(&p->table->print);
	printf("[%li] %i is sleeping\n", get_time() - p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	ft_usleep(p->table->ts);
	if (p->table->kill)
	{
		pthread_mutex_unlock(&p->table->killed);
		return (0);
	}
	pthread_mutex_lock(&p->table->print);
	printf("[%li] %i is thinking\n", get_time() - p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	if (p->table->kill)
	{
		pthread_mutex_unlock(&p->table->killed);
		return (0);
	}
	return (1);
}

int	eating(t_philo *p)
{
	p->status = EAT;
	p->count++;
	pthread_mutex_lock(&p->table->print);
	if (p->table->kill)
	{
		pthread_mutex_unlock(&p->table->fork[p->i - 1]);
		pthread_mutex_unlock(&p->table->fork[p->i]);
		pthread_mutex_unlock(&p->table->print);
		pthread_mutex_unlock(&p->table->killed);
		return (0);
	}
	printf("[%li] %i is eating\n", get_time() - p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	p->start_eat = get_time();
	ft_usleep(p->table->te);
	pthread_mutex_unlock(&p->table->fork[p->i - 1]);
	pthread_mutex_unlock(&p->table->fork[p->i]);
	return (1);
}

int	take_right_fork(t_philo *p)
{
	pthread_mutex_lock(&p->table->fork[p->i]);
	if (p->table->kill)
	{
		pthread_mutex_unlock(&p->table->fork[p->i]);
		pthread_mutex_unlock(&p->table->killed);
		return (0);
	}
	pthread_mutex_lock(&p->table->print);
	printf("[%li] %i has taken a fork\n", get_time()
		- p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	return (1);
}

int	take_left_fork(t_philo *p)
{
	pthread_mutex_lock(&p->table->fork[p->i - 1]);
	if (p->table->kill)
	{
		pthread_mutex_unlock(&p->table->fork[p->i - 1]);
		pthread_mutex_unlock(&p->table->killed);
		return (0);
	}
	pthread_mutex_lock(&p->table->print);
	printf("[%li] %i has taken a fork\n", get_time()
		- p->table->start_prog, p->i);
	pthread_mutex_unlock(&p->table->print);
	if (p->table->num_phil == 1)
	{
		ft_usleep(p->table->td);
		pthread_mutex_unlock(&p->table->fork[p->i - 1]);
		return (0);
	}
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
	while (!p->table->kill && p->status != DIE)
	{
		if (p->table->kill)
			return (0);
		if (!take_left_fork(p))
			break ;
		if (!take_right_fork(p))
			break ;
		if (!eating(p))
			break ;
		if (!sleeping(p))
			break ;
	}
	pthread_join(died, NULL);
	return (NULL);
}
