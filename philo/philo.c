#include "philo.h"

uint64_t	get_time()
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

void	*check_death(void *p)
{
	t_philo *philo;
	philo = (t_philo *)p;

	while (1)
	{
		if ((get_time() - philo->start_eat) > philo->table->td)
		{
			philo->kill = 1;
			pthread_mutex_lock(&philo->table->killed);
			philo->status = DIE;
			pthread_mutex_lock(&philo->table->print);
			printf("[%lli] phil [%i] é morto\n", get_time() - philo->table->start_prog, philo->i);		
			//pthread_mutex_unlock(&philo->table->print);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

void	*routine(void *philo)
{
	t_philo *p;
	p = (t_philo *)philo;
	pthread_t died;

	//pthread_mutex_lock(&p->table->print);
	pthread_create(&died, NULL, &check_death, (void *)philo);
	//pthread_mutex_unlock(&p->table->print);
	if (p->i % 2 == 0)
		ft_usleep(p->table->te);
	printf("p->kill di %i é %i\n", p->i, p->kill);
	while (!p->kill)
	{
		pthread_mutex_lock(&p->table->fork[p->i]);

		pthread_mutex_lock(&p->table->print);
		printf("[%lli] phil [%i] ha preso la forchetta a sinistra\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);

		pthread_mutex_lock(&p->table->fork[p->i + 1]);

		pthread_mutex_lock(&p->table->print);
		printf("[%lli] phil [%i] ha preso la forchetta a destra\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);

		p->status = EAT;
		pthread_mutex_lock(&p->table->print);
		printf("[%lli] phil [%i] sta mangiando\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);
		p->start_eat = get_time();

		ft_usleep(p->table->te);
		pthread_mutex_unlock(&p->table->fork[p->i]);
		pthread_mutex_unlock(&p->table->fork[p->i + 1]);
		p->status = SLEEP;

		pthread_mutex_lock(&p->table->print);
		printf("[%lli] phil [%i] sta dormendo\n",get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);

		ft_usleep(p->table->ts);

		pthread_mutex_lock(&p->table->print);
		printf("[%lli] phil [%i] sta pensando...\n", get_time() - p->table->start_prog, p->i);
		pthread_mutex_unlock(&p->table->print);
	}
	pthread_join(died, NULL);
	return (NULL);
}

int main(int argc, char *argv[])
{
	t_philo **philo;
	t_table *table;

	int	i = 0;

	if (argc < 4)
		return (0);
	table = malloc(sizeof(t_table));
	table->num_phil = ft_atoi(argv[1]);
	table->td = ft_atoi(argv[2]); 
	table->te = ft_atoi(argv[3]); 
	table->ts = ft_atoi(argv[4]); 
	table->fork = calloc(table->num_phil + 1, sizeof(pthread_mutex_t));

	philo = malloc(table->num_phil * sizeof(t_philo *));
	while (i < table->num_phil)
	{
		philo[i] = malloc(sizeof(t_philo));
		philo[i]->table = table;
		pthread_mutex_init(&table->fork[i], NULL);
		philo[i]->status = THINK;
		philo[i]->kill = 0;    
		philo[i]->start_eat = get_time();
		philo[i]->i = i + 1;
		i++;
	}
	table->fork[i] = table->fork[0];
	pthread_mutex_init(&table->print, NULL);
	pthread_mutex_init(&table->killed, NULL);
	table->start_prog = get_time();
	
	i = 0;
	while (i < table->num_phil)
	{ 
		pthread_create(&philo[i]->thread, NULL, &routine, (void *)philo[i]);
		i++;
	}
	i = 0;
	while (i < table->num_phil)
	{
		pthread_join(philo[i]->thread, NULL);
		if (philo[i]->kill)
			return (0);
		i++;
	}
	return (0);
}