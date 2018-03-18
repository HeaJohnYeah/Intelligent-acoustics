#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/syscall.h>

#include "auto.h"

int auto_mode = 0; //开启多媒体模式1，关闭多媒体模式0

static pthread_t auto_thread = 0;



void* AutoProcess(void *param)
{
	while(!auto_mode)
	{
		printf("auto mode is opening\n");
		usleep(1000);
	}
}
//多媒体初始化
int auto_init()
{
	printf("go into auto init !!!\n");
	return pthread_create(&auto_thread,NULL,AutoProcess,NULL);

}
//退出多媒体释放内存
void auto_fini()
{
	auto_mode = 1;
	
	if(0 != auto_thread)
	{
		pthread_join(auto_thread,NULL);
		auto_thread = 0;
	}
	
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */