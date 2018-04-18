#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif   /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <math.h>

#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h>

#include <errno.h>
#include <netdb.h>

//#include "sndfile.h"

#include "../common/msr.h"
#include "../common/auto.h"
#include "../common/socket.h"
#include "../common/common.h"
#include "../common/pthreadpool.h"
#include "../common/AIpython.h"

int req_mode = 0;
static pthread_t msr_thread = 0;
RecordInfo rest_info;//录音参数


PoolCreate api_request;//线程池

//设置格式
const MSR_U8 *FORMAT[24] ={
		"S8","U8",
		"S16_LE","S16_BE",
		"U16_LE","U16_BE",
		"S24_LE","S24_BE",
		"U24_LE","U24_BE",
		"S32_LE","S32_BE",
		"U32_LE","U32_BE",
		"FLOAT_LE","FLOAT_BE",
		"FLOAT64_LE","FLOAT64_BE",
		"IEC958_SUBFRAME_LE",
		"IEC958_SUBFRAME_BE",
		"MU_LAW","A_LAW",
		"IMA_ADPCM","MPEG GSM"
};

//文件类型
const MSR_U8 *FILE_TYPE[4]={
		"voc","wav","raw","au"
};


int Matoi(char *p)
{
    int temp = 0;
    if('0' <= *p && *p <= '9'){
     	 temp = temp + (*p - '0');
    }
    return temp;
}


//录音命令参数初始化
static int res_info_init(RecordInfo *rest,	MSR_U32 rate, MSR_U8 duration, MSR_U8 channels,const MSR_U8 *format, const MSR_U8 *type)
{
	if(NULL == rest){
		printf("request malloc fail\n");
		return -1;
	}
	rest->rate = rate;
	rest->duration = duration;
	rest->channels = channels;
	memcpy(rest->format, format, sizeof(rest->format));
	memcpy(rest->type, type, sizeof(rest->type));
	rest->record_param = RECORD_FINISH ;
	return 0;
}

static int python_cmd(void)
{
	pid_t pid;
	int status;
	pid = fork();
	if(pid == -1)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
		int ret = BaiduRequest(AI_SOUND_MOD, SOUND_FUN, NULL);
		printf("msr ret is %d\n",ret);
		return 0;
    }
    else if(pid > 0)
    {
        waitpid(pid, &status, 0);
        printf("this is parent process\n");
		return 0;
    }
    return 0;
}

//录音命令设置
static void record_cmd(RecordInfo *rest_info, MSR_U8 *cmd)
{
	sprintf(cmd, " arecord -c %d -d %d -f %s -r %d -t %s ../wav/record.%s  &",
				rest_info->channels, rest_info->duration,
				rest_info->format,rest_info->rate,
				rest_info->type,rest_info->type);
}

//录音命令线程
static void *request_process(void *arg)
{

	int client , reclen;
	char recvbuf[100];
	int recvbuf_len = sizeof(recvbuf);
	int ret;

	int serverSocket = TcpServerCreate(REC_SERVER_PORT, HOST);//服务器socket初始化
	if(serverSocket < 0)
		return NULL;

	MSR_U8 recmd[CMD_SIZE] = {0};
	record_cmd(&rest_info, recmd);
	if(NULL == recmd)
		printf("record cmd fail\n");

	while(!req_mode)
	{
		client = SocketAccept(serverSocket, 3000);//接收客户
		if(client < 0)
			continue;
		memset(recvbuf, 0, recvbuf_len);
		if(SocketRecv(client, recvbuf, recvbuf_len, 1000)){
			reclen = strlen(recvbuf);
			if(1 == reclen){
				rest_info.record_param = Matoi(&recvbuf[0]);
				printf("cmd : %s\n",recmd);
			}
			if(rest_info.record_param == RECORD_START){
				system(recmd);
				sleep(3);
				rest_info.record_param = RECORD_FINISH;
		 	 }
		}
		python_cmd();
	}
}

MSR_API int msr_init(int task_num)
{
	Ai_Requset_pool_init(&api_request,task_num);	//创建线程来录音
	//Python_init(); //创建python环境


	MSR_U32 rate 	 = 16000;//频率
	MSR_U8 duration = 2;//持续时间，秒为单位
	MSR_U8 channels = 1;//通道号

	int i,ret;
	if(0 != ret){
		printf("request info init fail task number is %d\n",i);
		msr_fini();
		//free(rest_info);
		return MSR_FAIL;
	}

    for (i = 0; i < task_num; i++) {
       	ret = res_info_init(&rest_info, rate, duration, channels, FORMAT[2], FILE_TYPE[1]);
		if(0 != ret){
			printf("request info init fail task number is %d\n",i);
			goto MSR_DISTORY;
		}

        ret = Ai_Requset_pool_add(&api_request, request_process, NULL);
		if(0 != ret){
			printf("create request fail task number is %d\n",i);
			goto MSR_DISTORY;
		}
    }

	return pthread_create(&msr_thread,NULL,request_process,NULL);

MSR_DISTORY:
	return MSR_FAIL;

}

MSR_API void msr_fini()
{
	Ai_Requset_pool_destroy(&api_request);
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
