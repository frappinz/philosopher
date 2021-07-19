#include "philo.h"

void	init_philo(t_table *table, t_philo **philo)
{
	int	i;

	i = 0;
	while (i < table->num_phil)
	{
		philo[i] = malloc(sizeof(t_philo));
		philo[i]->table = table;
		table->fork[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(table->fork[i], NULL);
		philo[i]->status = THINK;
		philo[i]->btdt = 0;
		philo[i]->count = 0;
		philo[i]->start_eat = get_time();
		philo[i]->i = i + 1;
		i++;
	}
	table->fork[i] = table->fork[0];
}

void	init_table(t_table *table, int argc, char *argv[])
{
	int		i;

	i = 0;
	table->num_phil = ft_atoi(argv[1]);
	table->td = ft_atoi(argv[2]);
	table->te = ft_atoi(argv[3]);
	table->ts = ft_atoi(argv[4]);
	table->max_eat = 0;
	if (argc == 6)
		table->max_eat = ft_atoi(argv[5]);
	table->finish = 0;
	table->kill = 0;
	table->fork = malloc((table->num_phil + 1) * sizeof (pthread_mutex_t *));
}

int	check_arguments(int argc, char *argv[])
{
	int	i;
	int	y;

	i = 1;
	y = 0;
	if ((argc < 5 || argc > 6) || (argv[1][0] == '0' && argv[1][1] == '\0'))
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

int	main(int argc, char *argv[])
{
	t_philo	**philo;
	t_table	*table;
	int		i;

	if (!check_arguments(argc, argv))
		return (0);
	table = malloc(sizeof(t_table));
	init_table(table, argc, argv);
	philo = malloc((table->num_phil + 1) * sizeof(t_philo *));
	init_philo(table, philo);
	pthread_mutex_init(&table->print, NULL);
	pthread_mutex_init(&table->killed, NULL);
	table->start_prog = get_time();
	i = -1;
	while (++i < table->num_phil)
		pthread_create(&philo[i]->thread, NULL, &routine, (void *)philo[i]);
	i = -1;
	//pthread_mutex_unlock(&table->killed);
	while (++i < table->num_phil)
		pthread_join(philo[i]->thread, NULL);
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