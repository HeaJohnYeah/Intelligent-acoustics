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

#include "../common/auto.h"
#include "../common/kernel_list.h"

#define MUSIC_DIR "../music"

typedef struct list_head music_list;

typedef struct {
	AUT_U8     *music_id;//音乐名字
	AUT_U8     *music_path;//音乐地址
	music_list *list;
}MusicInfo;

int auto_mode = 0; //开启多媒体模式1，关闭多媒体模式0
static pthread_t auto_thread = 0;
MusicInfo music_info_list;



void MusicInit(MusicInfo *music_node)//音乐初始化
{
	AUT_U8 MusicID_buf[100];
	struct dirent *music_dirent;
	DIR *music_dir = opendir(MUSIC_DIR);
	while(NULL != (music_dirent = readdir(music_dir))){//读取文件并去除"."和".."
		if(strcmp(music_dirent->d_name,".")
		||strcmp(music_dirent->d_name,".."))
			continue;
		
		sprintf(MusicID_buf, "%s%s", MUSIC_DIR, music_dirent->d_name);
		MusicInfo *new_music = malloc(sizeof(MusicInfo));
		
		memcpy(new_music->music_id,music_dirent->d_name,sizeof(music_dirent->d_name));
		memcpy(new_music->music_path,MusicID_buf,sizeof(MusicID_buf));
		list_add_tail(new_music->list,music_node->list);//增加到音乐链表
	}
	close_dir(music_dir);
}


void MusicDel(MusicInfo *music_node)
{
	
	//	
}

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
	MusicInit(&music_info_list);
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
