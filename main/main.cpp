#ifndef _WIN32
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#endif
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
#include "../auto/auto.h"

int main(int argc, char* argv[])
{
	if(0 != auto_init())
		auto_fini();
	printf("%s %d\n",__FUNCTION__,__LINE__);
	return 0;
}