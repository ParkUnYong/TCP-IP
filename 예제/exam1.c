#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main()
{
   char message[BUF_SIZE];
   int len = 0;
   int i = 0;

   fputs("input :",stdout);
   fgets(message[0],sizeof(message[0]),stdin);

   len = atoi(message);
   
   
   for(i=0;i<len;i++)
   {
      printf("operand %d :",i+1);
      fgets(message[i+1],sizeof(message[i+1]),stdin);
   }
   for(i=0;i<len;i++)
   {
      printf("숫자%d\n",message[i]);
   }

   return 0;
}
