#include "bin_sem.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#define KEY ((key_t)(1236))

int file;

int main()
{
	int i=0;
	int buf;
	char str[100];

	int semid = binary_semaphore_allocation(111, 0666 | IPC_CREAT);
	int semid2 = binary_semaphore_allocation(112, 0666 | IPC_CREAT);
	binary_semaphore_initialize(semid);
	binary_semaphore_initialize(semid2);

	int segment_id;
	char *shared_memory;
	struct shmid_ds shmbuffer;
	const int shared_segment_size = 60000;

	segment_id = shmget (KEY, shared_segment_size, IPC_CREAT | 0666);
	if (segment_id < 0) {printf ("ошибка шаред памяти\n"); return -1;}
	shared_memory = (char*) shmat (segment_id, 0, 0);
    printf ("\nСоздал шаред память\n");

	file = open("kek", O_RDONLY);
	if (file == -1) {printf ("ошибка файла\n"); return -1;}
    printf ("\nОткрыл файл\n");
	while (1)
    {
    	if (semid > 0)
    	{
    		printf ("\nДошёл до семафора %d\n", semid);
    		binary_semaphore_take(semid);
    		buf = read (file, shared_memory, 2);
    		i++;
    		if (buf == -1)
    		{
    				printf ("\nОшибка чтения из файла\n");
    				break;
    		}
    		//sprintf (shared_memory, "%s", str);
    		binary_semaphore_free(semid2);
    		printf ("\nФришнул семафору %d\n", semid2);
    	}
    	else printf ("\nПечалька с семафорой %d\n", semid);
    }
    printf ("\nВсего итераций: %d\n", i);
    sprintf (shared_memory, "@END!!END@");
    binary_semaphore_take(semid);
    binary_semaphore_free(semid2);
    printf ("\nФришнул семафору %d\n", semid2);
    return 0;
}