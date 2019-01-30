#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc,char *argv[])
{
  int serv_sock,clnt_sock;
  struct sockaddr_in serv_adr,clnt_adr;
  struct timeval timeout;
  fd_set reads,cpy_reads;

  socklen_t adr_sz;
  int fd_max,str_len,fd_num,i;
  char buf[BUF_SIZE];
  if(argc!=2){
    printf("Usage : %s <port>\n",argv[0]);
    exit(1);
  }

  serv_sock=socket(PF_INET,SOCK_STREAM,0);
  memset(&serv_adr,0,sizeof(serv_adr));
  serv_adr.sin_family=AF_INET;
  serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_adr.sin_port=htons(atoi(argv[1]));

  if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
    error_handling("bind() error!");
  if(listen(serv_sock,5)==-1)
    error_handling("listen() error!");

  FD_ZERO(&reads); // 인자로 전달된 fdset형 변수의 모든변수 0으로
  FD_SET(serv_sock,&reads); // fdset형 변수의 주소에 파일디스크립터 정보 세팅 1로 변함
  fd_max=serv_sock; //
  printf("fdmax 값 : %d\n",fd_max);


  while(1)
  {
    cpy_reads=reads; // 셋이 발동되면 변화된 디스크립터 말고 죄다 0이라 정보 카피
    timeout.tv_sec=5; // 타임 아웃할 초
    timeout.tv_usec=5000;// 타임 아웃 할 미리초

    if((fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout))==-1)
      break;  //셀렉트 실패시 빠져나감.
    if(fd_num==0)
      continue;//타임아웃 동안 변화 안일어나면 다시 실행. 타임아웃에 의한 종료시 0반환이라.

    for(i=0;i<fd_max+1,i++)
    {
      if(FD_ISSET(i,&cpy_reads)) // select 함수가 1이상을 반환했을때 실행됨. 어느 디스크립터인지를 찾음.
      {
        if(i==serv_sock) // i의 디스크립터 값이 서버소켓과 일치시 즉 서버소켓에 연결요청이 왔을때
        {
          adr_sz=sizeof(clnt_adr);
          clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
          FD_SET(clnt_sock,&reads); //클라이언트 소켓에 대한 정보 새롭게 reads에 세팅.
          if(fd_max<clnt_sock)
	        fd_max=clnt_sock;
          printf("connected client : %d\n",clnt_sock);
         }
      }
       else
       {
          str_len=read(i,buf,BUF_SIZE);
          if(str_len==0)
          {
            FD_CLR(i,&reads);
            close(i);
            printf("closed client : %d\n",i);
          }

          else
          {
            wtire(i,buf,str_len); //echo!
          }
        }
       }
      }
   }
   close(serv_sock);
   return 0;
}


void error_handling(char *buf)
{
   fputs(buf,stderr);
   fputc('\n',stderr);

}
