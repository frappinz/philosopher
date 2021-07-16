#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

enum e_status
{
	DIE,
	SLEEP,
	EAT,
	THINK
};


typedef struct	s_table
{
	//int 				i;
	uint64_t 			td;
	uint64_t 			te;
	uint64_t 			ts;
	int					**forchetta;
	pthread_mutex_t    	*fork;
	pthread_mutex_t    	print;
	uint64_t			start_prog;
	int 				num_phil;
	pthread_mutex_t    	killed;
}t_table;

typedef	struct s_philo
{
	int					kill;
	pthread_t		thread;
	int				i;
	int				status;
	uint64_t		start_eat;
	uint64_t		start_sleep;
	t_table			*table;
}t_philo;

int	ft_atoi(const char *str);
int main(int argc, char *argv[]);

#endif