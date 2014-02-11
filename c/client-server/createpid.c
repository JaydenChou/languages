#include      <sys/types.h>
#include      <sys/stat.h>
#include      <stdio.h>
#include      <stdlib.h>
#include      <string.h>
#include      <unistd.h>
#include      <time.h>
#include      <fcntl.h>

#define ZCHILDS 4
pid_t childs[ZCHILDS];

#define NO 0
#define SI 1

int main(void)
{
	int i = 0;
	int j = 0;
	int haylugar = SI;
	for (i = 0; i < ZCHILDS; i++) {
		childs[i] = -1;
	}
	while (1) 
	{
		pid_t pid;
		haylugar = NO;
		for (i = 0; i < ZCHILDS; i++) {
			if (childs[i] == -1) {
				haylugar = SI;
			}
		}
		
		if (haylugar == SI) {
			printf("HAY LUGAR\n");
			if ((pid = fork()) < 0) {
				perror("fork error");
			} else if (pid == 0) {
				/* this is the child */
				for (j = 1; j < 5; j++) {
					//printf("child, parent_pid = %d, child pid = %d... zzz\n", getppid(), getpid());
					sleep(3);
				}
				printf("\n");
				exit(0);
			}
			/* I'm the father */
			printf("Father\n");
			int status;
			for (i = 0; i < ZCHILDS; i++) {
				if (childs[i] == -1) {
					printf("Saving pid %d in %d\n", pid, i);
					childs[i] = pid;
					break;
				}
			}
			for (i = 0; i < ZCHILDS; i++) {
				if (childs[i] != -1) {
					if (waitpid(childs[i], &status, WNOHANG) > 0) { /* wait for first child */
						if (WIFEXITED(status)) {
							int retorno = WEXITSTATUS(status);
							printf("status pid %d = %d\n", childs[i], retorno);
							childs[i] = -1;
						}
					}
				}
			}
		} else {
			printf("NO HAY LUGAR\n");
		}
		if (haylugar == NO) {
			for (j = 1; j < 5; j++) {
				printf("I am the parent pid = %d (%d,%d)\n", getpid(), i, j);	
				sleep(1);
			}
		}
		
	}

	exit(0);
}
