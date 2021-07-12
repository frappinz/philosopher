#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef	struct	s_timeval	
{
	time_t	tv_sec;
	suseconds_t	tv_usec;
}t_timeval;

typedef struct	s_var
{
	int i;
	int num_phil;
	suseconds_t td;
	suseconds_t te;
	suseconds_t ts;
	pthread_t *thread;
	pthread_mutex_t    *mutex;
} t_var;

int	ft_atoi(const char *str);
int main(int argc, char *argv[]);

#endif