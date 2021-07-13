#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

// typedef	struct	s_timeval	
// {
// 	time_t	tv_sec;
// 	suseconds_t	tv_usec;
// }t_timeval;

enum e_status
{
	DIE,
	SLEEP,
	EAT,
	THINK
};

typedef	struct s_philo
{
	pthread_t		thread;
	enum e_status	status;
	int				index;

}t_philo;

typedef struct	s_table
{
	int 				i;
	int 				num_phil;
	suseconds_t 		td;
	suseconds_t 		te;
	suseconds_t 		ts;
	int					*forchetta;
	t_philo				*philo;
	pthread_mutex_t    	*fork;
	pthread_mutex_t    	print;
	pthread_mutex_t		aspee;
}t_table;

int	ft_atoi(const char *str);
int main(int argc, char *argv[]);

#endif