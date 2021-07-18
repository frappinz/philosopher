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


void	ft_exit(t_table *table, t_philo **philo)
{
	int	i;
	int num;

	num  = table->num_phil;
	i = 0;
	while (i < num)
	{
		free(philo[i]);
		i++;
	}
	free(philo);
	free(table->fork);
	free(table);
}

int	check_arguments(int argc, char *argv[])
{
	int	i;
	int	y;

	i = 1;
	y = 0;
	if ((argc < 4 || argc > 6) || (argv[1][0] == '0' && argv[1][1] == '\0'))
	{
		printf("Invalid arguments\n");
		return (0);
	}
	while (i < argc)
	{
		y = 0;
		while (argv[i][y])
		{
			if (argv[i][y] < '0' || argv[i][y] > '9')
			{
				printf("Invalid arguments\n");
				return (0);
			}
			y++;
		}
		i++;
	}
	return (1);
}

int main(int argc, char *argv[])
{
	t_philo **philo;
	t_table *table;
	int		i;
	
	i = 0;

	if (!check_arguments(argc, argv))
		return (0);
	table = malloc(sizeof(t_table));
	table->num_phil = ft_atoi(argv[1]);
	table->td = ft_atoi(argv[2]); 
	table->te = ft_atoi(argv[3]); 
	table->ts = ft_atoi(argv[4]);
	table->max_eat = 0;
	if (argc == 6)
		table->max_eat = ft_atoi(argv[5]);
	table->finish = 0;
	table->fork = malloc(table->num_phil + 1 * sizeof(pthread_mutex_t));
	table->kill = 0;
	philo = malloc(table->num_phil * sizeof(t_philo *));
	while (i < table->num_phil)
	{
		philo[i] = malloc(sizeof(t_philo));
		philo[i]->table = table;
		pthread_mutex_init(&table->fork[i], NULL);
		philo[i]->status = THINK;
		philo[i]->btdt = 0;
		philo[i]->count = 0;
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
		i++;
	}
	ft_exit(table, philo);
	return (0);
}

// 11000011 << 2
// 00001100 >> 2
// 00000011

// OPTION = 0

// LEFT = 1 = 2ˆ0
// RIGHT = 2 = 2ˆ1
// PRINT = 4 = 2ˆ2
// DEATH = 8 = 2ˆ3

// OPTION = OPTION | PRINT
// OPTION = OPTION | DEATH

// if (OPTION & LEFT)
// 	unlock left
// if (OPTION & RIGHT)
// 	unlock right
// opt = 1 ˆ 5 = 1

// 1 = left
// 2 = right
// 4 = print
// & 
// |
// ˆ
// <<
// >>