#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){

	pid_t pid;
	int fd[2];
	char str[6];
	size_t size;

	printf("I'm parrent before fork %d\n", getpid());
	
	if(pipe(fd) < 0){
		printf("Can't pipe\n");
		exit(-1);
	}

	switch(pid = fork()){
		case -1:
			printf("fork error\n");
			exit(-1);
		case 0:
			printf("Child PID after fork %d\n", getpid());
			close(fd[0]);
			size = write(fd[1], "Hello", 6);
			if(size != 6){
				printf("Can't write\n");
				exit(-1);
			}
			printf("Child exit\n");
		default:
			printf("Parrent PID after fork %d\n", getpid());
			close(fd[1]);
			size = read(fd[0],	str, 6);
			if(size < 0){
				printf("Can't read\n");
				exit(-1);
			}
			printf("Parrent exit\n"); 
			printf("str = %s\n", str);
			close(fd[0]);
	}
}
