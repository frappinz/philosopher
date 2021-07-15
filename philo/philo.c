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

void	*routine(void *philo)
{
	t_philo *p;
	p = (t_philo *)philo;

	//printf("stato filosofo %i\n", p[p->i].status);
	while (p[p->i].status != DIE)
	{
		//printf("%i\n", p->table->fork[p->i - 1]);
		pthread_mutex_lock(&p->table->print);
		pthread_mutex_lock(&p->table->fork[p->i - 1]);
		printf("phil [%i] ha preso la forchetta a sinistra\n", p->i);
		pthread_mutex_unlock(&p->table->print);
		if (p->table->forchetta[p->i - 1])
			p->table->forchetta[p->i - 1] = 0;
		pthread_mutex_lock(&p->table->fork[p->i]);
		if(p->table->forchetta[p->i])
		{
			pthread_mutex_lock(&p->table->print);
			printf("phil [%i] ha preso la forchetta a destra\n", p->i);
			pthread_mutex_unlock(&p->table->print);
			p->table->forchetta[p->i] = 0;
			p[p->i].status = EAT;
			pthread_mutex_lock(&p->table->print);
			printf("phil [%i] sta mangiando\n", p->i);
			pthread_mutex_unlock(&p->table->print);
			ft_usleep(p->table->te);
			pthread_mutex_unlock(&p->table->fork[p->i - 1]);
			pthread_mutex_unlock(&p->table->fork[p->i]);
			p->table->forchetta[p->i - 1] = 0;
			p->table->forchetta[p->i] = 0;
			p[p->i].status = SLEEP;
			pthread_mutex_lock(&p->table->print);
			printf("phil [%i] sta dormendo\n", p->i);
			pthread_mutex_unlock(&p->table->print);
			ft_usleep(p->table->ts);
			pthread_mutex_lock(&p->table->print);
			printf("phil [%i]sta pensando...\n", p->i);
			pthread_mutex_unlock(&p->table->print);
		}

	}
	return (NULL);
}

// void	*routine(void *table)
// {
// 	t_table *cast_table;
// 	cast_table = (t_table *)table;
// 	//cast_table->philo[cast_table->i].status = THINK;
// 	while (cast_table->philo[cast_table->i].status != DIE)
// 	{
// 		pthread_mutex_lock(&cast_table->print);
// 		printf("il filosofo n. %i ", cast_table->i);
// 		pthread_mutex_unlock(&cast_table->print);
// 		if(cast_table->philo[cast_table->i].status == THINK) //DEVE MANGIARE
// 		{
// 			pthread_mutex_lock(&cast_table->fork[cast_table->i]);
// 			if (*cast_table->forchetta[cast_table->i]) //se esiste la forchetta davanti
// 			{
// 				pthread_mutex_lock(&cast_table->fork[cast_table->i + 1]);
// 				if (*cast_table->forchetta[cast_table->i + 1])
// 				{
// 					*cast_table->forchetta[cast_table->i] = 0;
// 					*cast_table->forchetta[cast_table->i + 1] = 0;
// 					cast_table->philo[cast_table->i].status = EAT;
// 					pthread_mutex_lock(&cast_table->print);
// 					printf("mangia\n");
// 					pthread_mutex_unlock(&cast_table->print);
// 					ft_usleep(cast_table->te);
// 					pthread_mutex_unlock(&cast_table->fork[cast_table->i + 1]);
// 					pthread_mutex_unlock(&cast_table->fork[cast_table->i]);
// 					*cast_table->forchetta[cast_table->i] = 1;
// 					*cast_table->forchetta[cast_table->i + 1] = 1;
// 					pthread_mutex_lock(&cast_table->print);
// 					printf("rilascia le forchette numero %i e %i\n", cast_table->i, cast_table->i + 1);
// 					pthread_mutex_unlock(&cast_table->print);
// 				}
// 				else
// 				{
// 					pthread_mutex_unlock(&cast_table->fork[cast_table->i + 1]);
// 					pthread_mutex_unlock(&cast_table->fork[cast_table->i]);
// 				}
// 			}
// 			else
// 			{
// 				pthread_mutex_unlock(&cast_table->fork[cast_table->i]);
// 				//printf("dorme\n");
// 				//cast_table->philo[cast_table->i].status = SLEEP;
// 				//ft_usleep(cast_table->te);
// 			}
// 		}
// 		else if (cast_table->philo[cast_table->i].status == EAT)
// 		{
// 			pthread_mutex_lock(&cast_table->print);
// 			printf("dorme\n");
// 			pthread_mutex_unlock(&cast_table->print);
// 			cast_table->philo[cast_table->i].status = SLEEP;
// 			ft_usleep(cast_table->ts);
// 		}
// 		else if (cast_table->philo[cast_table->i].status == SLEEP)
// 		{
// 			pthread_mutex_lock(&cast_table->print);
// 			printf("pensa\n");
// 			pthread_mutex_unlock(&cast_table->print);
// 			cast_table->philo[cast_table->i].status = THINK;
// 		}
// 		else
// 		{
// 			//pthread_mutex_lock(&cast_table->aspee);
// 			printf("non ho fatto nulla\n");
// 			//cast_table->philo[cast_table->i].status = DIE;
// 			//exit(0);
// 		}
// 	}
// 	printf("sono uscito dal while\n");
// 	return (NULL);
// }

int main(int argc, char *argv[])
{
	t_philo *philo;
	t_table table;

	int	i = 0;

	if (argc < 4)
		return (0);
	table.num_phil = ft_atoi(argv[1]);
	table.td = ft_atoi(argv[2]); 
	table.te = ft_atoi(argv[3]); 
	table.ts = ft_atoi(argv[4]); 
	table.fork = calloc(table.num_phil + 1, sizeof(pthread_mutex_t));
	table.forchetta = calloc(table.num_phil + 1, sizeof(int *));
	philo = calloc(table.num_phil, sizeof(t_philo));
	while (i < table.num_phil)
	{
		philo[i].table = &table;
		i++;
	}
	pthread_mutex_init(&table.print, NULL);
	i = 0;
	while (i < table.num_phil)
	{
		table.forchetta[i] = malloc(sizeof(int));
		*table.forchetta[i] = 1;
		pthread_mutex_init(&table.fork[i], NULL);
		philo[i + 1].status = THINK;
		i++;
	}
	table.forchetta[i] = table.forchetta[0];
	table.fork[i] = table.fork[0];
	philo->i = 1;
	table.start_prog = get_time();
	while (philo->i < table.num_phil)
	{ 
		pthread_create(&philo[philo->i].thread, NULL, &routine, philo);
		sleep(1);
		philo->i++;
	}
	// // while (phi < table->num_phil)
	// // {
	// // 	pthread_join(philo[philo.i].thread, NULL);
	// // 	i++;
	// // }
	// return (0);
}