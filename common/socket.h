#ifndef _SOCKET__H_
#define _SOCKET__H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <stdint.h>


#define MSG_DONT_WAIT 0
#define socklen_t int

#define HOST "127.0.0.1"
#define PYTHON_SERVER_PORT 6666
#define CGI_SERVER_PORT 8117
#define REC_SERVER_PORT 8000

#define SOCKET_API
#define SOCKET_FAIL   (-1)

#ifdef __cplusplus
extern "C" {
#endif

SOCKET_API int TcpServerCreate(int port ,const char *ip);
SOCKET_API int SocketSelect(int sockfd,int timeout_ms);
SOCKET_API int SocketAccept(int sockfd,int timeout_ms);
SOCKET_API int SocketSend(int sockfd, void *buf, int len, int timeout_ms);
SOCKET_API int SocketRecv(int sockfd, void *buf, int len, int timeout_ms);

#ifdef __cplusplus
}
#endif



#endif
