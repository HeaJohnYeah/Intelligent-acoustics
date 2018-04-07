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
#include "../common/list.h"



typedef struct MusicINF{
	struct MusicINF  *next;
	struct MusicINF  *prev;
	AUT_U8 *music_path;//音乐地址
	AUT_U8 *music_id;//音乐名字
	AUT_U8  Reserved[10];//预留参数
}MusicInfo;


int auto_mode = 0; //开启多媒体模式1，关闭多媒体模式0
static pthread_t auto_thread = 0;
MusicInfo *music_head;
MusicInfo *music_flag;

static int music_pause_flag = 0;

//./madplay -o wav:- ../1.mp3 | aplay



static void music_list_add(MusicInfo *music_node, AUT_U8 *id, AUT_U8 *path)
{
	MusicInfo *new=malloc(sizeof(MusicInfo));
	MusicInfo *p= music_node->prev;
	new->music_id = id;
	new->music_path = path;
	music_node->prev = new;
	new->next = music_node;
	p->next = new;
	new->prev = p;
}


static void music_list_init(MusicInfo *music_node, AUT_U8 *id, AUT_U8 *path)
{
	music_node = malloc(sizeof(MusicInfo));
	music_node->music_id = id;
	music_node->music_path = path;
	music_node->next = music_node;
	music_node->prev = music_node;
}

static void music_list_del(MusicInfo *music_node)
{
	MusicInfo *p;
	for(p = music_node->next; p != music_node; p = p->next){
		p->prev->next = p->next;
		p->next->prev = p->prev;
		p->next = p;
		p->prev = p;
	}
}

static int  aplay_cmd(MusicInfo *music_info, int apcmd)//播放命令
{
	AUT_U8 cmd[100];
	switch(apcmd)
	{
		case MUSIC_PAUSE://暂停
			sprintf(cmd," killall -STOP aplay"); 
			music_pause_flag = 1;
			break;
			
		case MUSIC_PLAY ://播放
			if(music_pause_flag){
				sprintf(cmd," killall -CONT aplay"); 
				music_pause_flag = 0;
				sprintf(cmd, "madplay -o wav:- %s | aplay &", music_info->music_path);
			}
			else{
				sprintf(cmd, "madplay -o wav:- %s | aplay &", music_info->music_path);
			}
			break;
			
		case MUSIC_NEXT ://下一首
				system("killall -9 aplay");
				music_info = music_info->next;
				sprintf(cmd, "madplay -o wav:- %s | aplay &", music_info->music_path);
			break;
			
		case MUSIC_PREV  ://上一首
				system("killall -9 aplay");
				music_info = music_info->prev;
				sprintf(cmd, "madplay -o wav:- %s | aplay &", music_info->music_path);
			break;
			
		case MUSIC_STOP ://停止
				system("killall -9 aplay");
				return AUTO_SUCCESS;
		
		default :
			return AUTO_FAIL;
	}	
	Sleep(5);
	printf("%s\n",cmd);
	system(cmd);
	return AUTO_SUCCESS;
}


static void MusicInit(MusicInfo *music_node)//音乐初始化
{
	int first_init = 1;
	AUT_U8 MusicID_buf[100];
	struct dirent *music_dirent;
	DIR *music_dir = opendir(MUSIC_DIR);
	while(NULL != (music_dirent = readdir(music_dir))){//读取文件并去除"."和".."
		if(strcmp(music_dirent->d_name,".")
		&& strcmp(music_dirent->d_name,".."))
		{
			sprintf(MusicID_buf, "%s/%s", MUSIC_DIR, music_dirent->d_name);
			if(music_dirent->d_type==8)
			{
				if(first_init){
					music_list_init(music_node,music_dirent->d_name,MusicID_buf);
					first_init = 0;
				}
				/*增加到音乐链表*/
				music_list_add(music_node,music_dirent->d_name,MusicID_buf);
			}
		}
	}
	close_dir(music_dir);
}


static void MusicDel(MusicInfo *music_node)
{
	MusicInfo *qd;
	music_list_del(music_node);

}

AUTO_API void MusicControl(int apcmd)
{
	int ret = aplay_cmd(music_flag,apcmd);
	if(ret == AUTO_FAIL){
		printf("error cmd!!!\n");
	}
}


static void* AutoProcess(void *param)
{
	while(!auto_mode)
	{
		Sleep(1000);
	}
}
//多媒体初始化
AUTO_API int auto_init(void)
{
	MusicInit(music_head);
	return pthread_create(&auto_thread,NULL,AutoProcess,NULL);

}
//退出多媒体释放内存
AUTO_API void auto_fini(void)
{
	auto_mode = 1;
	MusicDel(music_head);
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
