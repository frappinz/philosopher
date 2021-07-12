#include "philo.h"

// int N = -1;

// void	*routine(void *i)
// {
// 	N++;
// 	printf("\n%d: sono il thread n %d!\n", (int)i, N);
// 	return(NULL);
// }

// int main(int ac, char **av)
// {
// 	pthread_t thread[5];
// 	int i = 0;
// 	int y;

// 	while (i < 5)
// 	{
// 		printf("bella ho creato thread %d\n", i);
// 		y = pthread_create(&thread[i], NULL, routine, (void *)i);
// 		if (y)
// 		{
// 			printf("Errore!!!!!!!! il ritorno é %d\n", y);
// 			exit (-1);
// 		}
// 		pthread_join(thread[i], NULL);
// 		i++;
// 	}
// 	return (0);
// }

// #define NUMTHRDS 10
// pthread_t callThd[NUMTHRDS];
// pthread_mutex_t mutexdata; int data;

// void *decrementa(void *arg)
// {
// 	pthread_mutex_lock (&mutexdata);
// 	if(data>0)
// 		data--;
// 	pthread_mutex_unlock (&mutexdata);
// 	pthread_exit((void*) 0);
// }

// int main (int argc, char *argv[])
// {
// 	int i; int status; pthread_attr_t attr;
// 	data=4;
// 	pthread_mutex_init (&mutexdata, NULL);
// 	for(i=0;i < NUMTHRDS;i++) 
// 	{
// 		// per default i thread consentono il join,
// 		pthread_create ( &callThd[i], NULL, decrementa, (void *)i);
// 	}
// 	for(i=0;i < NUMTHRDS;i++) 
// 	{ // aspetto la fine dei thread	
// 		pthread_join ( callThd[i], (void **)&status);
// 	}
// 	printf ("data = %d \n", data);
// 	pthread_mutex_destroy (&mutexdata);
// 	pthread_exit(NULL);
// } 

void	*routine(void *var)
{
	//(int)i;
	t_var *cast_var;
	cast_var = (t_var *)var;

	pthread_mutex_lock(&cast_var->mutex[cast_var->i]);
	printf("ho creato %i filosof*\n", cast_var->i);
	

	return (NULL);
}

int main(int argc, char *argv[])
{
	t_var var;
	int i = 1;
	struct timeval	start;
	struct timeval	end;
	
	if (argc < 4)
		return (0);
	//pthread_mutex_init(&var.mutex, NULL);
	var.num_phil = ft_atoi(argv[1]);
	var.td = ft_atoi(argv[2]);
	var.te = ft_atoi(argv[3]);
	var.ts = ft_atoi(argv[4]);
	var.thread = calloc(var.num_phil, sizeof(pthread_t *));
	var.mutex = calloc(var.num_phil, sizeof(pthread_mutex_t *));

	// gettimeofday(&start, NULL);
	// sleep(1);
	// gettimeofday(&end, NULL);
	// printf("sono passati %li microsecondi\n", ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)));
	//printf("h: %ld,%d\nnumber_of_philosophers: %i\ntime_to_die: %d\ntime_to_eat %d\ntime_to_sleep %d\n", current_time.tv_sec, current_time.tv_usec, num_phil, td, te, ts);
	while (var.i <= var.num_phil)
	{
		pthread_mutex_init(&var.mutex[var.i], NULL);
		//pthread_join(thread[i], NULL); STA COSA FA AVERE A TUTTI LO STESSO ID!!
		//printf("thread[%i]  = %i\n", i, thread[i]);
		i++;
	}
	i = 0;
	while (var.i <= var.num_phil)
	{
		pthread_create(&var.thread[var.i], NULL, routine, &var);
		pthread_join(var.thread[var.i], NULL);
		//printf("thread[%i]  = %i\n", i, thread[i]);
		var.i++;
	}
	return (0);
}