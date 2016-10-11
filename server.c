#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int UnionTCPIPServer(int port)
{
	int  m_listenfd = 0;
	struct sockaddr_in	LocalAddr;
	int  m_uPort = 0;
	//int recvbuff = 1024*1024 ;//1M
	//int sendbuff = 1024*1024 ;
	int Option = 1;//reuse addr
	int ret = 0;

	m_listenfd = socket(AF_INET,SOCK_STREAM,0);
	if( m_listenfd < 0 )
	{
		printf("ERROR: Create Listen Socket Failed.\n");
		return -1;
	}
	memset((char *)&LocalAddr,0,sizeof(struct sockaddr_in));
	//LocalAddr.sin_family = AF_INET;
	//LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//
	inet_pton(AF_INET ,"0.0.0.0" , &LocalAddr.sin_addr); 
	LocalAddr.sin_port = htons(port);
	
	//setsockopt(m_listenfd, SOL_SOCKET, SO_RCVBUF, (char *)&recvbuff, sizeof(recvbuff)) ;
	//setsockopt(m_listenfd, SOL_SOCKET, SO_SNDBUF, (char *)&sendbuff, sizeof(sendbuff)) ;
	//setsockopt(m_listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&Option, sizeof(int));
	

	if( bind(m_listenfd,(struct sockaddr *) &LocalAddr,sizeof(struct sockaddr_in)) < 0 )
	{
		printf("ERROR: Bind SvrPort(%d) Failed!\n",  port);
		return -1;
	}
	
	return m_listenfd;
}

int main(int argc ,char *argv[])
{
    if(argc!=2)
		err_ret("usage: name  port");
	int port = atoi(argv[1]);

	int ListenFd;
	if (( ListenFd = UnionTCPIPServer(port) ) <0 )
		err_ret("UnionTCPIPServer err");
	
	if( listen(ListenFd, 1) <0 )
		err_ret("listen err");

	printf("accept once\n");
	//接收客户端请求
    struct sockaddr_in clnt_addr;	
    socklen_t clnt_addr_size = sizeof(clnt_addr);
	memset(&clnt_addr , 0 , clnt_addr_size);
    int clnt_sock = accept(ListenFd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	
	char recvbuf[8192] = {0};
	if( recv(clnt_sock,recvbuf,100 , 0) <0)
		err_sys("recv err");

	printf("recv:[%s]\n" , recvbuf);

#define SENDBUFLEN (1024*1024*100)
#define LOOPTIMES  (1)
	char *sendbuf = NULL;
	sendbuf = malloc(SENDBUFLEN);//100M

	unsigned long start;
	unsigned long end;
	double total;
	start = GetTickCount();

	int i = 0;
	for( ; i<LOOPTIMES ; i++)  // 100次 , 共10GB
	{
		if( send(clnt_sock, sendbuf ,SENDBUFLEN , 0) <0)
			err_ret("send err\n");
				

	}

	end = GetTickCount();
	total =  end - start;

	double speed;
	speed = (LOOPTIMES*SENDBUFLEN*8)/(total/1000)/(1024*1024);
	printf("speed=[%f]Mb/s",speed);
	
    freeBuf(sendbuf);
				
	return 0;
}