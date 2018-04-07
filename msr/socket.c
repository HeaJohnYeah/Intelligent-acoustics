#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  /* End of #ifdef __cplusplus */


#include <stdio.h>  
#include <unistd.h>  
#include <string.h>  
#include <stdlib.h>  
#include <arpa/inet.h>  
#include <sys/socket.h>
#include <sys/types.h>  
#include <netinet/in.h> 
#include <sys/un.h> 

#include "../common/socket.h"


SOCKET_API int TcpServerCreate(int port ,const char *ip)
{
	int serverSocket = socket(AF_INET,SOCK_STREAM,0);
	if(serverSocket < 0){
		perror("socket create fault");
		return -1;
	}
	//创建												
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);//端口号
	serverAddr.sin_addr.s_addr = inet_addr(ip);//地址
	//绑定
	if(bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0){
		perror("bind socket fault");
		return -1;
	}
	//监听
	if(listen(serverSocket,5)<0){
		perror("listen fault");
		return -1;
	}
	//返回socket
	return serverSocket;
}

/*socket select*/
SOCKET_API int SocketSelect(int sockfd,int timeout_ms)
{
	int ret;
	fd_set fds;
	struct timeval tv;
	FD_ZERO(&fds);			//设置socket参数
	FD_SET(sockfd,&fds);
	
	tv.tv_sec = timeout_ms / 1000;	//延迟设置
	tv.tv_usec = (timeout_ms % 1000) * 1000;
	
	ret = select(sockfd + 1, &fds, NULL, NULL, &tv);
	if((ret <= 0) || !FD_ISSET(sockfd, &fds))
	{
		//close(sockfd);
		return -1;
	}
	return sockfd;
}
/*socket接收*/
SOCKET_API int SocketAccept(int sockfd,int timeout_ms)
{
	int ret , acceptfd;
	
	struct sockaddr_in addr;	
	socklen_t size = sizeof(struct sockaddr_in);
	
	sockfd = SocketSelect(sockfd,timeout_ms);
	if(-1 == sockfd)
		return -1;
	acceptfd = accept(sockfd,(struct sockaddr*)&addr,&size);
	if(-1 == acceptfd)
		return -1;
	
	return acceptfd;
}

SOCKET_API int SocketSend(int sockfd, void *buf, int len, int timeout_ms)
{
	int sentd = 0, ret;
	while(sentd < len)
	{
		ret = send(sockfd,(const char*)buf + sentd,len - sentd, MSG_NOSIGNAL | MSG_DONT_WAIT);
		if(ret > 0)
		{
			sentd += ret;
		}
		else
		{
			sockfd = SocketSelect(sockfd, timeout_ms);
			if(sockfd < 0)
				break;
		}
	}
}

SOCKET_API int SocketRecv(int sockfd, void *buf, int len, int timeout_ms)
{
	int recvd = 0, ret;
	
	while(recvd < len)
	{
		ret = recv(sockfd, (char *)buf+recvd, len-recvd, MSG_DONT_WAIT);
		if (ret >0)
		{
			recvd += ret;
		}
		else if (0 == ret)
		{
			return -2;
		}
		else
		{
			sockfd = SocketSelect(sockfd, timeout_ms);//如果没有接收到消息或者错误则退出
			if(sockfd < 0)
				break;
		}	
	}
	return 1;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

