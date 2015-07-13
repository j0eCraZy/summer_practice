#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#define SIG SIGUSR1
#define SIG2 SIGQUIT

void handler(int sig, siginfo_t *si, void *uc) {
	printf("Caught signal %d, sending PID: %ld\n", sig, (long)si->si_pid);
}

void *signalthreed(void *num){
	struct sigaction sa;
	printf("Establishing handler for signal %d and %d\n", SIG, SIG2);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIG, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	if (sigaction(SIG2, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	while(1);
}

int main(int argc, char *argv[]) {
	sigset_t mask;
	pthread_t thread;
	int rc;

	rc=pthread_create(&thread,NULL, signalthreed, (void *)0);
	if(rc){
		printf("ERROR: %d\n",rc);
		exit(-1);
	} 

	pthread_join(thread,NULL);

	
	return 0;
}