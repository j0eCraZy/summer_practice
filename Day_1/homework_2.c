#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


	int main(int argc, char* argv[]) 
	{
		int ex;
		int pid;
		int fd[2];
		pipe(fd);
		int i;
		char buf[900]="";
		int a;
		switch(pid=fork())
		{
			case -1:
				printf("Error fork\n");
				return -1;

			case 0: 
				printf("Child start\n");
				close(fd[0]);
				dup2(fd[1],STDOUT_FILENO);	
				ex = execl("/bin/ls","ls", "-l", "/tmp/", NULL);
				if (ex == -1) 
					perror("execl");
				printf("Child finished\n");
				exit(EXIT_SUCCESS);   

			default: 
				printf("Parrent start\n");
				close(fd[1]);
				read(fd[0],buf,sizeof(buf));
				for (i=0;i<500;i++) 
					printf("%c ", buf[i]);
				printf("\n");
				wait(&a);
				printf("Parrent finished\n");
			}
	}
