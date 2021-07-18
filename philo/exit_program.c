#include "philo.h"

int	check_eaten(t_philo *philo)
{	
	if (philo->count == philo->table->max_eat && !philo->btdt)
	{
		pthread_mutex_lock(&philo->table->print);
		philo->table->finish++;
		philo->btdt = 1;
		pthread_mutex_unlock(&philo->table->print);
	}
	if (philo->table->finish == philo->table->num_phil)
	{
		pthread_mutex_lock(&philo->table->killed);
		pthread_mutex_lock(&philo->table->print);
		pthread_mutex_unlock(&philo->table->print);
		philo->table->kill = 1;
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
		{
			if (!check_eaten(philo))
				break ;
		}
		if ((get_time() - philo->start_eat) > philo->table->td)
		{
			pthread_mutex_lock(&philo->table->killed);
			philo->table->kill = 1;
			philo->status = DIE;
			pthread_mutex_lock(&philo->table->print);
			printf("[%li] %i died\n", get_time()
				- philo->table->start_prog, philo->i);
			pthread_mutex_unlock(&philo->table->print);
			break ;
		}
		usleep(100);
	}
	return (NULL);
}
