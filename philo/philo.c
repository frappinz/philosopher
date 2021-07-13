#include "philo.h"

void	*routine(void *table)
{
	t_table *cast_table;
	cast_table = (t_table *)table;

	cast_table->philo[cast_table->i].status = 6;
	//pthread_mutex_init(&cast_table->aspee, NULL);
	//printf("\n");
	while (cast_table->philo[cast_table->i].status != 0)
	{
		printf("il %i filosof* ", cast_table->i);
		if(cast_table->philo[cast_table->i].status == 6 || cast_table->philo[cast_table->i].status == THINK) //DEVE MANGIARE
		{
			if (cast_table->forchetta[cast_table->i]) //se esiste la forchetta davanti
			{
			//printf("la forchetta davanti a lui é libera\n");
				if (cast_table->i == cast_table->num_phil) //se é l'ultimo filosofo
				{
					if (cast_table->forchetta[0])	//se esiste la forchetta alla sua sinistra
					{
						//printf("ed é libera anche quella alla sua destra\n");
						pthread_mutex_lock(&cast_table->fork[0]);	//pigliala!!
						cast_table->forchetta[0] = 0;
						pthread_mutex_lock(&cast_table->fork[cast_table->i]);
						cast_table->forchetta[cast_table->i] = 0;
						cast_table->philo[cast_table->i].status = EAT;
						printf("MANGIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
						usleep(cast_table->te);
						pthread_mutex_unlock(&cast_table->fork[0]);
						pthread_mutex_unlock(&cast_table->fork[cast_table->i]);

					}
					else
					{
						printf("dorme\n");
						cast_table->philo[cast_table->i].status = SLEEP;
						usleep(cast_table->ts);
					}
				}
				else if (cast_table->forchetta[cast_table->i + 1])
				{
					//printf("ed é libera anche quella alla sua destra\n");
					pthread_mutex_lock(&cast_table->fork[cast_table->i]);
					pthread_mutex_lock(&cast_table->fork[cast_table->i + 1]);
					cast_table->forchetta[cast_table->i] = 0;
					cast_table->forchetta[cast_table->i + 1] = 0;
					cast_table->philo[cast_table->i].status = EAT;
					printf("MANGIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
					usleep(cast_table->te);
					pthread_mutex_unlock(&cast_table->fork[cast_table->i]);
					pthread_mutex_unlock(&cast_table->fork[cast_table->i + 1]);
					cast_table->forchetta[cast_table->i] = 1;
					cast_table->forchetta[cast_table->i + 1] = 1;
				}
				else
				{
					printf("dorme\n");
					cast_table->philo[cast_table->i].status = SLEEP;
					usleep(cast_table->ts);
				}
			}
			else
			{
				if (cast_table->philo[cast_table->i].status == 6)
				{
					printf("pensa\n");
					cast_table->philo[cast_table->i].status = THINK;
				}
				else
				{
					printf("dorme\n");
					cast_table->philo[cast_table->i].status = SLEEP;
					usleep(cast_table->ts);
				}
			}
		}
		else if (cast_table->philo[cast_table->i].status == EAT)
		{
			printf("dorme\n");
			cast_table->philo[cast_table->i].status = SLEEP;
			usleep(cast_table->ts);
		}
		else if (cast_table->philo[cast_table->i].status == SLEEP)
		{
			printf("pensa\n");
			cast_table->philo[cast_table->i].status = THINK;
		}
		else
		{
			pthread_mutex_lock(&cast_table->aspee);
			printf("sono morto\n");
			cast_table->philo[cast_table->i].status = DIE;
			exit(0);
		}
	}
	return (NULL);
}

int main(int argc, char *argv[])
{
	t_table *table;
	int	i;
	// struct timeval	start;
	// struct timeval	end;
	
	if (argc < 4)
		return (0);
	table = malloc(sizeof(t_table));
	table->i = 0;
	table->num_phil = ft_atoi(argv[1]);
	table->td = ft_atoi(argv[2]);
	table->te = ft_atoi(argv[3]);
	table->ts = ft_atoi(argv[4]);
	table->philo = calloc(table->num_phil, sizeof(t_philo));
	table->fork = calloc(table->num_phil, sizeof(pthread_mutex_t));
	table->forchetta = calloc(table->num_phil, sizeof(int));
	i = 0;
	while (i < table->num_phil)
	{
		table->forchetta[i] = 1;
		i++;
	}
	while (table->i < table->num_phil)
	{
		pthread_mutex_init(&table->fork[table->i], NULL);
		table->i++;
	}
	table->i = 0;
	//pthread_mutex_init(&table->aspee, NULL);
	while (table->i < table->num_phil)
	{ 
		//pthread_mutex_lock(&table->aspee);
		pthread_create(&table->philo[table->i].thread, NULL, &routine, table);
		usleep(10);
		pthread_join(table->philo[table->i].thread, NULL);
		table->i++;
		//pthread_mutex_unlock(&table->aspee);
	}
	return (0);
}
	
	// gettimeofday(&start, NULL);
	// sleep(1);
	// gettimeofday(&end, NULL);
	// printf("sono passati %li microsecondi\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
	//printf("h: %ld,%d\nnumber_of_philosophers: %i\ntime_to_die: %d\ntime_to_eat %d\ntime_to_sleep %d\n", current_time.tv_sec, current_time.tv_usec, num_phil, td, te, ts);