
//
// Este es el Cliente
//
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "clisrvnet.h"


#define TRUE 1
#define FALSE 0

struct linger options = { TRUE, 60 };

#ifdef OK
# undef OK
#endif
#ifdef ERROR
# undef ERROR
#endif

#define OK  0
#define ERROR  -1

#define ERROR_TIMEOUT -2

int main(int argc,char *argv[])
{
   char   buff[BUFFSIZE];
   int      sock;

   sock = createClient();

   while (1) {
      char   buff2[BUFFSIZE];
      scanf("%s", buff);

      if (!strlen(buff))
         continue;

      printf("sending [%s](%d)\n", buff, strlen(buff));
      if (send(sock,buff,sizeof(buff),0) < 0)
         break;

      if (!strcmp(buff, "FIN"))
         break;


      if (buff[0] != '?')
         continue;

      while (1) {
         int n;
         memset(buff2, 0, sizeof(buff2));
         n = recv(sock,buff2,BUFFSIZE,0);
         if (n) break;
         if (strlen(buff2)) break;
      }
      printf("Evento [%s] -> estado %s\n", buff, buff2);

   }

   close(sock);

   return 0;
}
