/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fminardi <fminardi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/20 17:40:47 by fminardi          #+#    #+#             */
/*   Updated: 2021/07/20 17:40:48 by fminardi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <stdint.h>

enum e_status
{
	DIE,
	SLEEP,
	EAT,
	THINK
};

typedef struct s_table
{
	uint64_t			td;
	uint64_t			te;
	uint64_t			ts;
	uint64_t			start_prog;
	int					num_phil;
	int					max_eat;
	int					finish;
	int					kill;
	pthread_mutex_t		**fork;
	pthread_mutex_t		print;
	pthread_mutex_t		killed;
	pthread_t			thread;
}t_table;

typedef struct s_philo
{
	int				btdt;
	int				count;
	int				i;
	int				status;
	uint64_t		start_eat;
	pthread_t		thread;
	t_table			*table;
}t_philo;

int			ft_atoi(const char *str);
void		*routine(void *philo);
void		ft_usleep(int time);
uint64_t	get_time(void);
void		ft_exit(t_table *table, t_philo **philo);
void		*check_death(void *p);

#endif