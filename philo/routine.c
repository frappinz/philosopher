#include "philo.h"

void	*check_death(void *p)
{
	t_philo *philo;
	philo = (t_philo *)p;

	while (1)
	{
		if (philo->table->max_eat)
		{
			if (philo->count == philo->table->max_eat && !philo->btdt)
			{
				pthread_mutex_lock(&philo->table->print);
				philo->table->finish++;
				printf("[%li] phil [%i] ha mangiato abbastanza finish è %i, count è %i\n", get_time() - philo->table->start_prog, philo->i, philo->table->finish, philo->count);		
				philo->btdt = 1;
				pthread_mutex_unlock(&philo->table->print);
			}
			if (philo->table->finish == philo->table->num_phil)
			{
				pthread_mutex_lock(&philo->table->killed);
				pthread_mutex_lock(&philo->table->print);
				printf("FINISHHHHHH %i\n", philo->table->finish);
				pthread_mutex_unlock(&philo->table->print);
				philo->table->kill = 1;
				return NULL ;
			}
		}
		if ((get_time() - philo->start_eat) > philo->table->td)
		{
			pthread_mutex_lock(&philo->table->killed);
			philo->table->kill = 1;
			philo->status = DIE;
			pthread_mutex_lock(&philo->table->print);
			printf("[%li] phil [%i] é morto\n", get_time() - philo->table->start_prog, philo->i);		
			pthread_mutex_unlock(&philo->table->print); 
			break ;
		}
		usleep(100);
	}
	return (NULL);
}

void	*routine(void *philo)
{
	t_philo *p;
	p = (t_philo *)philo;
	pthread_t died;

	pthread_create(&died, NULL, &check_death, (void *)philo);
	if (p->i % 2 == 0)
		ft_usleep(p->table->te);
	while (!p->table->kill && p->status != DIE)
	{
		if (p->table->kill)
			return (0);
		pthread_mutex_lock(&p->table->fork[p->i - 1]);
		pthread_mutex_lock(&p->table->print);
		if (p->table->kill)
		{
			pthread_mutex_unlock(&p->table->fork[p->i - 1]);
			pthread_mutex_unlock(&p->table->print);
			pthread_mutex_unlock(&p->table->killed);
			return NULL;
		}
		printf("[%li] phil [%i] ha preso la forchetta a sinistra\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);
		if (p->table->num_phil == 1)
		{
			ft_usleep(p->table->td);
			pthread_mutex_unlock(&p->table->fork[p->i - 1]);
			return NULL;
		}
		pthread_mutex_lock(&p->table->fork[p->i]);
		pthread_mutex_lock(&p->table->print);
		if (p->table->kill)
		{
			pthread_mutex_unlock(&p->table->fork[p->i]);
			pthread_mutex_unlock(&p->table->print);
			pthread_mutex_unlock(&p->table->killed);
			return (0);
		}
		printf("[%li] phil [%i] ha preso la forchetta a destra\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);
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
		printf("[%li] phil [%i] sta mangiando\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);
		p->start_eat = get_time();
		ft_usleep(p->table->te);
		pthread_mutex_unlock(&p->table->fork[p->i - 1]);
		pthread_mutex_unlock(&p->table->fork[p->i]);
		p->status = SLEEP;
		pthread_mutex_lock(&p->table->print);
		if (p->table->kill)
		{
			pthread_mutex_unlock(&p->table->print);
			pthread_mutex_unlock(&p->table->killed);
			return (0);
		}
		printf("[%li] phil [%i] sta dormendo\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);
		ft_usleep(p->table->ts);
		pthread_mutex_lock(&p->table->print);
		if (p->table->kill)
		{
			pthread_mutex_unlock(&p->table->print);
			pthread_mutex_unlock(&p->table->killed);
			return (0);
		}
		printf("[%li] phil [%i] sta pensando...\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);
		if (p->table->kill)
		{
			pthread_mutex_unlock(&p->table->killed);
			return (0);
		}
	}
	pthread_join(died, NULL);
	return (NULL);
}
