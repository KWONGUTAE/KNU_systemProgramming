/* incprint.c - one thread increments, the other prints */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM	5

int	counter = 0;

void main(void)
{
	pthread_t t1;		/* one thread */
	void	*print_msg(void *);/* its function */
	int	i;

	pthread_create(&t1, NULL, print_msg, NULL);
	for ( i = 0; i<NUM; i++)
	{
		counter++;
		sleep(1);
	}

	pthread_join(t1, NULL);
}

void *print_msg(void *m)
{
	int i;
	for(i = 0; i<NUM; i++)
	{
		printf("count = %d\n", counter);
		sleep(1);
	}
	return NULL;
}

