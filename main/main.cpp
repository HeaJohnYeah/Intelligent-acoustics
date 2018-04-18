#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h> /* for ARPHRD_ETHER */
#include <errno.h>
#include <features.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/route.h>

#include <net/if_slip.h>
#include <stddef.h>

#include "../common/common.h"
#include "../common/auto.h"
#include "../common/msr.h"
#include "../common/socket.h"

int g_run = 1;

int Atoi(char *p)
{
    int temp = 0;
    if('0' <= *p && *p <= '9'){
     	 temp = temp + (*p - '0');
    }
    return temp;
}

int main(int argc, char* argv[])
{

	int client , reclen;
	ModeSelect apcmd;

	if(0 != auto_init()){
		auto_fini();
	}
	if(0 != msr_init(2)){
		msr_fini();
	}

	char recvbuf[100];
	int recvbuf_len = sizeof(recvbuf);

	int serverSocket = TcpServerCreate(PYTHON_SERVER_PORT, HOST);//服务器socket初始化
	if(serverSocket < 0)
		return SOCKET_FAIL;

	while(g_run)
	{
		client = SocketAccept(serverSocket, 3000);//接收客户
		if(client < 0)
			continue;
		memset(recvbuf, 0, recvbuf_len);
		if(SocketRecv(client, recvbuf, recvbuf_len, 1000)){
			reclen = strlen(recvbuf);
			printf("%s\n",recvbuf);
			/*音乐播放控制*/
			if(2 == reclen){
				apcmd.MusicCro = Atoi(&recvbuf[0]);
				apcmd.PlayMode = Atoi(&recvbuf[1]);
				MusicControl(apcmd);
			}
			else
				printf("Nothing recv!!!\n");
		}
	}
	return 0;
}