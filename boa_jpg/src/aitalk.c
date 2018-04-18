#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>

int socket_connect2(uint32_t ip, uint16_t port, int timeout_ms)
{
    int sockfd;
    struct sockaddr_in addr;

#ifdef _WIN32
	u_long arg = 1;
#else
	int flags;
#endif

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
        return -1;

#ifdef _WIN32
	ioctlsocket(sockfd, FIONBIO, &arg);
#else
    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#endif
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(port);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
    
    if (-1 == connect(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)))
    {
        fd_set fds;
        struct timeval tv;
        int ret, optval = 0;
		socklen_t optlen = sizeof(socklen_t);

        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);
        
        tv.tv_sec = 0;
        tv.tv_usec = timeout_ms * 1000;

        ret = select(sockfd + 1, NULL, &fds, NULL, &tv);
        if ((ret <= 0) || !FD_ISSET(sockfd, &fds))
        {
            close(sockfd);
            return -1;
        }
        
        ret = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen);
        if ((0 != ret) || (0 != optval))
        {
            close(sockfd);
            return -1;
        }
    }

#ifndef _WIN32
    fcntl(sockfd, F_SETFL, flags);
#endif

    return sockfd;
}

int socket_connect(const char *ip, uint16_t port, int timeout_ms)
{
    return socket_connect2(inet_addr(ip), port, timeout_ms);
}

int socket_send(int sockfd, const void *buf, int len, int timeout_ms)
{
    int sent = 0, ret;

    while (sent < len)
    {
        ret = send(sockfd, (const char*)buf + sent, 
			len - sent, MSG_NOSIGNAL | MSG_DONTWAIT);
		
        if (ret > 0)
        {
            sent += ret;
        }
        else
        {
            fd_set fds;
            struct timeval tv;
            
			if (EWOULDBLOCK != errno)
                return -1;
            
            FD_ZERO(&fds);
            FD_SET(sockfd, &fds);

            tv.tv_sec = 0;
            tv.tv_usec = timeout_ms * 1000;
            
            ret = select(sockfd + 1, NULL, &fds, NULL, &tv);
            if ((ret <= 0) || !FD_ISSET(sockfd, &fds))
                break;
        }
    }
    
    return sent;
}

int AITALK_GET_FORM(const char* ip, void* msg, int size)
{

	int sockfd = socket_connect(ip, 8000, 1000);
	if(-1 == sockfd)
		return 0;
	socket_send(sockfd, msg, size, 1000);
	return 0;
}


int main()
{
	char *env, *buf, *pbuf;
	int len;
	printf("Content-type: text/html;charset=UTF-8\r\n\r\n");
	//获取请求数据
	env = getenv("CONTENT_LENGTH");
	if ((NULL == env) || ('\0' == env[0]))
		return 0;
		
	len = atoi(env);
	buf = (char*)malloc(len + 1);
	if (NULL == buf)
		return 0;

	pbuf = fgets(buf, len + 1, stdin);
	
	//解析key
	if (0 != strncmp(buf, "msg=", 4))
	{
		free(buf);
		return 0;
	}
	pbuf = buf + 4;
	
	
	
	if(0 == strncmp(pbuf, "record", 7))//暂停
	{
		char msg[2]  = "0";
		AITALK_GET_FORM("127.0.0.1", (void *)msg, sizeof(msg));
	}
	else if(0 == strncmp(pbuf, "shutdown", 9))//播放
	{
		char msg[2]  = "1";
		AITALK_GET_FORM("127.0.0.1", (void *)msg, sizeof(msg));
	}
	else
	{
		char msg[2]  = "2";
		AITALK_GET_FORM("127.0.0.1", (void *)msg, sizeof(msg));
	}

	return 0;
}
