#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
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
#include <pthread.h>  

#include "../common/common.h"
#include "../common/auto.h"
#include "../common/kernel_list.h"


typedef struct list_head music_list;

typedef struct {
	music_list node;
	AUT_U8 *music_id;//音乐名字
	AUT_U8 *music_path;//音乐地址
}MusicInfo;

int auto_mode = 0; //开启多媒体模式1，关闭多媒体模式0
static pthread_t auto_thread = 0;
music_list music_info_list;
MusicInfo *music_info;

static void aplay_cmd(MusicInfo *music_info, AUT_U8 *cmd)
{
	sprintf(cmd, "aplay  %s/%s",music_info->music_path, music_info->music_id);
}


static void MusicInit(music_list *music_node)//音乐初始化
{
	AUT_U8 MusicID_buf[100];
	music_list *node;
	/*指向链表头*/
	music_info = list_entry(music_node ,MusicInfo, node);
	
	struct dirent *music_dirent;
	DIR *music_dir = opendir(MUSIC_DIR);
	while(NULL != (music_dirent = readdir(music_dir))){//读取文件并去除"."和".."
		if(strcmp(music_dirent->d_name,".")
		||strcmp(music_dirent->d_name,".."))
			continue;

		sprintf(MusicID_buf, "%s/%s", MUSIC_DIR, music_dirent->d_name);
		MusicInfo *new_music = malloc(sizeof(MusicInfo));

		memcpy(new_music->music_id,music_dirent->d_name,sizeof(music_dirent->d_name));
		memcpy(new_music->music_path,MusicID_buf,sizeof(MusicID_buf));
		list_add_tail(&new_music->node,music_node);//增加到音乐链表
	}
	
	close_dir(music_dir);
}


static void MusicDel(music_list *music_node)
{

	music_list *node, *tmp;
	MusicInfo *qd;
	list_for_each_safe(node,tmp,music_node)
	{
		qd = list_entry(node ,MusicInfo, node);
		list_del(node);
		free(qd);
	}
}

AUTO_API void MusicControl(int apcmd)
{
	//AUT_U8 *apcmd;
	switch(apcmd)
	{
		case MUSIC_PAUSE:
				printf("pause!!!\n");
				break;
		case MUSIC_PLAY :
				printf("play!!!\n");
				break;
		case MUSIC_NEXT :
				printf("next!!!\n");
				break;
		case MUSIC_PRE  :
				printf("pre!!!\n");
				break;
	}
	//aplay_cmd(music_info, apcmd);
	//if(NULL == apcmd)
		//printf("apcmd cmd fail\n");
}


static void* AutoProcess(void *param)
{
	while(!auto_mode)
	{
		printf("auto mode is opening\n");
		Sleep(1000);
	}
}
//多媒体初始化
AUTO_API int auto_init(void)
{
	MusicInit(&music_info_list);
	return pthread_create(&auto_thread,NULL,AutoProcess,NULL);

}
//退出多媒体释放内存
AUTO_API void auto_fini(void)
{
	auto_mode = 1;
	MusicDel(&music_info_list);
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
