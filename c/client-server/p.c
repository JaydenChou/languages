#include <sys/types.h>
#include <sys/wait.h>
#include "ourhdr.h"

#define ZCHILDS 4
pid_t childs[ZCHILDS];

int main(void)
{
	memset(&childs, -1, sizeof(pid_t)*ZCHILDS);

	int i = 0;
	int j = 0;
	for (i = 1; i < ZCHILDS; i++) {
		pid_t pid;
		if ((pid = fork()) < 0) {
			perror("fork error");
		} else if (pid == 0) { /* first child */
			if ((pid = fork()) < 0) {
				perror("fork error");
			} else if (pid > 0) {
				exit(0);	/* parent from second fork == first child */
			}
			/* we are the second child */
			childs[i] = pid;
			printf("second child, parent_pid = %d, pid = %d... zzz\n", getppid(), getpid());
			for (j = 1; j < 20; j++) {
				printf(".");
				sleep(1);
			}
			printf("\n");
			exit(0);
		}
		/* we are the parent */
		int status;
		if (waitpid(childs[i], &status, 0) > 0) { /* wait for first child */
			if (WIFEXITED(status)) {
				int retorno = WEXITSTATUS(status);
				printf("status pid %d = %d\n", childs[i], retorno);
			}
		}
		
		for (j = 1; j < 20; j++) {
			printf("I am the parent pid = %d (%d,%d)\n", getpid(), i, j);	
			sleep(1);
		}
		
	}

	exit(0);
}
