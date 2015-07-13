#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3

int count=1;

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *counter(void *t)
{
	long my_id=(long)t;
	int slp;
	
	pthread_mutex_lock(&count_mutex);
	count++;
	slp=count;
	
	while (slp=sleep(slp));
	
	printf("#%ld Done! count= %d\n",my_id, count);
	
	if (count == 1+NUM_THREADS)
	{
		pthread_cond_broadcast(&count_threshold_cv);
		printf("signal\n");
	}

	pthread_cond_wait(&count_threshold_cv, &count_mutex);
	
	pthread_mutex_unlock(&count_mutex);
	
	pthread_cond_broadcast(&count_threshold_cv);
	printf("#%ld Success!\n", my_id);
	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	int i;
	int flag=1;
	long t;
	
	pthread_t threads[3];
	pthread_attr_t attr;
	
	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init(&count_threshold_cv, NULL);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	for (i=0; i< NUM_THREADS; i++)
	{
		t=i;
		pthread_create(&threads[i], &attr, counter, (void *)t);
	}
	
	for(i=0; i< NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
pthread_attr_destroy(&attr);
pthread_mutex_destroy(&count_mutex);
pthread_cond_destroy(&count_threshold_cv);
pthread_exit(NULL);
}
