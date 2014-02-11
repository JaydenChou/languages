#include      <sys/types.h>
#include      <sys/mman.h>
#include      <sys/stat.h>
#include      <stdio.h>
#include      <stdlib.h>
#include      <string.h>
#include      <unistd.h>
#include      <time.h>
#include      <fcntl.h>

#define ZCHILDS 4

enum { S_RUN = 0, S_STOP, S_TOTAL };

typedef struct {
   pid_t pid;
   int s;
   time_t t;
} tEvent;

#define  SIZE     (sizeof(tEvent))      /* size of shared memory area */

void init(tEvent *p)
{
   p->pid = -1;
   p->s = S_STOP;
   p->t = (time_t)-1;
}

void create(tEvent *p)
{
   p->pid = getpid();
   p->s = S_RUN;
   time(&p->t);
}

void update(tEvent *p, int s)
{
   p->s = s;
   time(&p->t);
   printf("guardo para pid %d -> status %d -> a las %d\n", p->pid, p->s, p->t);
}

int getStatus(tEvent *p, int last, int ps)
{
   int i, j;
   time_t t;
   time(&t);
   printf("getStatus: last = %d", last);
   t-=last;
   printf("getStatus: status = %d, t = %d", p->s, p->t);
   if (p->t >= t) {
     printf("getStatus: encontre !!! primer status = %d, t = %d", i, p->t);
     ps = p->s;
   }
   printf("getStatus: NO encontre !!! status retorno el primero");

   return j;
}

#define NO 0
#define SI 1

int main(void)
{
	int i = 0;
	int j = 0;
	int haylugar = SI;
   	tEvent  *area[ZCHILDS];
	for (i = 0; i < ZCHILDS; i++) {
		printf("parent: mmap area");
	//	if ((area[i] = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == (tEvent*) -1) 
		if ((area[i] = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0)) == (tEvent*) -1) 
		{
			printf("parent: mmap error");
			exit(0);
		}
		init(area[i]);
	}
	while (1) 
	{
		for (i = 0; i < ZCHILDS; i++) {
			pid_t pid;
			haylugar = NO;
			if (area[i]->s == S_STOP) {
				haylugar = SI;
			} else {
				continue;
			}
			
			if (haylugar == SI) {
				printf("HAY LUGAR\n");
				if ((pid = fork()) < 0) {
					perror("fork error");
				} else if (pid == 0) {
					/* this is the child */
				        create(area[i]);
					for (j = 1; j < 5; j++) {
						sleep(3);
					}
				        update(area[i], S_STOP);
					exit(0);
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
	}

	exit(0);
}
