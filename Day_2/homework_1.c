#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 2
char password[50]="";
char password_OK[50]="lalaka123rechnaya";

void *enter_pass(void *num)
{
    int i=0;
    int str;
	printf("enter password: ");
	while ((str = getchar ( )) != '\n') 
		password [i++] = str;
	printf("\n You entered: %s\n", password);
	
}

void *pass_auth(void *num)
{	
	while (strcmp (password, "") == 0) 
		sleep(1);
	if (strcmp (password, password_OK)==0) 
		printf("Yep, yep\n");
	else printf("wrong pass\n");
}

int main(int argc, char *argv[])
{
	long i;
	pthread_t threads[NUM_THREADS];
	int rc;
	int rc2;
	rc= pthread_create(&threads[0], NULL, enter_pass, NULL);
	if (rc)
		{
			printf("ERROR; thread number  %d\n", rc);
			exit(-1);
		}
		
	rc2= pthread_create(&threads[1], NULL, pass_auth, NULL);
	if (rc2)
		{
			printf("ERROR; thread number  %d\n", rc);
			exit(-1);
		}	

	for (i = 0; i < NUM_THREADS; i++)
	{

		pthread_join(threads[i], NULL);
		printf("thread #%ld finished\n",i);
	}

	return 0;
}
