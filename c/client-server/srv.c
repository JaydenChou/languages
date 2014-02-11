#include <ctype.h>                              /* Manejo de caracteres       */
#include <errno.h>                              /* Manejo de errores de DECC  */
#include <stdio.h>                              /* Manejo de I/O standard     */
#include <limits.h>                             /* Tamaños de enteros         */
#include <unistd.h>                             /* Definiciones de POSIX      */
#include <sys/mman.h>                           /* Definiciones de POSIX      */

#include "errores.h"
#include "clisrvnet.h"


#define  EVENTS   100

enum { S_0 = 0, S_1, S_2, S_3, S_4, S_5, S_6, S_7, S_8, S_9, S_TOTAL };

typedef struct {
   pid_t pid;
   int s;
   time_t t[S_TOTAL];
} tEvent;

#define  SIZE     (sizeof(tEvent))      /* size of shared memory area */


void create(tEvent *p)
{
   int i;
   for (i = 0; i < S_TOTAL; i++) {
      p->s = -1;
   }
   p->pid = getpid();
   p->s = S_0;
   time(&p->t[p->s]);
}

void update(tEvent *p, int s)
{
   p->s = s;
   time(&p->t[p->s]);
   err_msg("guardo para pid %d -> status %d -> a las %d\n", p->pid, p->s, p->t[p->s]);
}

int getStatus(tEvent *p, int last, int (ps)[S_TOTAL])
{
   int i, j;
   time_t t;
   time(&t);
   err_msg("getStatus: last = %d", last);
   t-=last;
   err_msg("getStatus: busco hasta status = %d", p->s);
   for (i = S_0, j = 0; i <= p->s; i++) {
      err_msg("getStatus: status = %d, t = %d", i, p->t[i]);
      if (p->t[i] >= t) {
         err_msg("getStatus: encontre !!! primer status = %d, t = %d", i, p->t[i]);
         ps[j++] = p->s;
      }
   }
   err_msg("getStatus: NO encontre !!! status retorno el primero");

   return j;
}


int main(int argc, char** argv)
{
   char   buff[BUFFSIZE];
   int    sock, n;
   tEvent  *area[EVENTS];

   sock = createServer();

   while (1) {
      pid_t pid;
      int iEvent;
      memset(buff, 0, BUFFSIZE);
      n = recv(sock,buff,BUFFSIZE,0);

      if (!n) continue;
      if (!strlen(buff)) continue;

      err_msg("parent: recv %d\n", n);

      buff[n-2] = 0;     // null terminate
      err_msg("parent: print msg [%s](%d)\n", buff, strlen(buff));

      iEvent = atoi(buff);

      if (iEvent) {
         err_msg("parent: mmap area");
         if ((area[iEvent] = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == (tEvent*) -1)
            err_sys("parent: mmap error");


         if (iEvent > 0 && iEvent < EVENTS) {
            /* crear evento */
            if ((pid = fork()) < 0) {
               err_sys("padre: for error");
            } else if (pid > 0) {         /* parent */
               err_msg("padre: in parent %d, child = %d", getpid(), pid);
            } else {                      /* child */
               err_msg("child%d: close sock %d\n", getpid(), sock);
               sockClose(sock);
               create(area[iEvent]);
               update(area[iEvent], S_1);
               sleep(10);
               update(area[iEvent], S_2);
               update(area[iEvent], S_3);
               {
               int x = 0; while(x++ < 100);
               update(area[iEvent], S_4);
               }

               while (1) {
                  {
                  time_t t;
                  time(&t);
                  srand(t);
                  switch(rand()%6) {
                     case 0: update(area[iEvent], S_5);
                             exit(0); break;
                     case 1: update(area[iEvent], S_6);
                             exit(1); break;
                     case 2: update(area[iEvent], S_7);
                             abort(); break;
                     case 3: update(area[iEvent], S_8);
                             kill(getpid(), SIGKILL); break;
                     case 4: update(area[iEvent], S_9);
                             {
                             int pepe=0;
                             pepe = iEvent/pepe;
                             break;
                             }
                     case 5: /* do nothing */
                  }
                  }
               }
               err_msg("child%d: close\n", getpid());
            }
         }
      } else {
         /* info ?*/
         char   buff2[BUFFSIZE];
         int estado;
         int s[S_TOTAL];
         int ss;
         int sp=0;
         int total;
         if (buff[0] != '?')
            continue;
         memset(buff2, 0, BUFFSIZE-1);
         memcpy(buff2, &buff[1], strlen(buff)-1);
         buff2[strlen(buff)-1] = 0;
         iEvent = atoi(buff2);
         err_msg("parent: buscando status de evento %d\n", iEvent);
         total = getStatus(area[iEvent], 2, s);

         memset(buff2, 0, BUFFSIZE-1);
         sp += sprintf(buff2, "evento = %d - ", iEvent);
         for (ss = 0; ss < total; ss++)
            sp += sprintf(buff2 + sp, "st[%d],", estado);
         err_msg("parent: sending (%s)\n", buff2);
         if (send(sock,buff2,sizeof(buff2),0) < 0)
            break;

         err_msg("parent: enviado\n");
      }
   }

   exit(0);
}

