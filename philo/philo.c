/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fminardi <fminardi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 17:40:44 by fminardi          #+#    #+#             */
/*   Updated: 2021/07/21 12:37:12 by fminardi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	init_table(t_table *table, int argc, char *argv[])
{
	int		i;

	i = 0;
	table->num_phil = ft_atoi(argv[1]);
	table->td = ft_atoi(argv[2]);
	table->te = ft_atoi(argv[3]);
	table->ts = ft_atoi(argv[4]);
	if (table->ts == 0 || table->td == 0 || table->te == 0)
		return (0);
	table->max_eat = 0;
	if (argc == 6)
	{
		table->max_eat = ft_atoi(argv[5]);
		if (table->max_eat == 0)
			return (0);
	}
	table->finish = 0;
	table->kill = 0;
	table->fork = malloc((table->num_phil + 1) * sizeof (pthread_mutex_t *));
	pthread_mutex_init(&table->print, NULL);
	pthread_mutex_init(&table->killed, NULL);
	return (1);
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
	if (!init_table(table, argc, argv))
	{
		printf("Invalid argument\n");
		free(table);
		return (0);
	}
	philo = malloc((table->num_phil) * sizeof(t_philo *));
	init_philo(table, philo);
	table->start_prog = get_time();
	i = -1;
	while (++i < table->num_phil)
		pthread_create(&philo[i]->thread, NULL, &routine, (void *)philo[i]);
	i = -1;
	while (++i < table->num_phil)
		pthread_join(philo[i]->thread, NULL);
	ft_usleep(100);
	ft_exit(table, philo);
	return (0);
}
