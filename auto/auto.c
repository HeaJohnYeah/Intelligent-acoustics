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
#include "../common/socket.h"


typedef struct MusicINF{
	AUT_U8 music_id[50];//音乐名字
	AUT_U8 music_path[100];//音乐地址
	struct MusicINF  *next;
	struct MusicINF  *prev;
}MusicInfo;


int auto_mode = 0; //开启多媒体模式1，关闭多媒体模式0
static pthread_t auto_thread = 0;
MusicInfo *music_head;
MusicInfo *music_flag;

static int music_pause_flag = 0;

//./madplay -o wav:- ../1.mp3 | aplay

int Atoi(char *p)
{
    int temp = 0;
    if('0' <= *p && *p <= '9'){
     	 temp = temp + (*p - '0');
    }
    return temp;
}


void music_list_add(MusicInfo *music_node, AUT_U8 *id, AUT_U8 *path)
{
	MusicInfo *p;
	MusicInfo *new = (MusicInfo *)malloc(sizeof(MusicInfo));
	p = music_node->prev;
	memcpy(new->music_id,id, sizeof(new->music_id));
	memcpy(new->music_path, path, sizeof(new->music_path));
	music_node->prev = new;
	new->next = music_node;
	p->next = new;
	new->prev = p;
}


void music_head_init(MusicInfo *music_node, AUT_U8 *id, AUT_U8 *path)
{
	memcpy(music_node->music_id,id, sizeof(music_node->music_id));
	memcpy(music_node->music_path, path, sizeof(music_node->music_path));
	music_node->next = music_node;
	music_node->prev = music_node;
}

void music_list_del(MusicInfo *music_node)
{
	MusicInfo *p;
	for(p = music_node->next; p != music_node; p = p->next){
		p->prev->next = p->next;
		p->next->prev = p->prev;
		p->next = p;
		p->prev = p;
		free(p);
	}
}


int play(AUT_U8 *path)
{
	AUT_U8 cmd[100];
	sprintf(cmd, "madplay -q  %s < /dev/null &",path);
	system(cmd);
}


int  aplay_cmd(int apcmd)//播放命令
{
	AUT_U8 cmd[100];
	int ret;
	switch(apcmd)
	{
		case MUSIC_PAUSE://暂停
			system("killall -STOP madplay");
			music_pause_flag = 1;
			break;

		case MUSIC_PLAY ://播放
			if(music_pause_flag){
				system(" killall -CONT madplay ");
				music_pause_flag = 0;
				play(music_flag->music_path);
			}
			else{
				ret = play(music_flag->music_path);
			}
			break;

		case MUSIC_NEXT ://下一首
				system("killall -9 madplay ");
				music_flag = music_flag->next;
				play(music_flag->music_path);
			break;

		case MUSIC_PREV  ://上一首
				system("killall -9 madplay ");
				music_flag = music_flag->prev;
				play(music_flag->music_path);
			break;

		case MUSIC_STOP ://停止
				system("killall -9 madplay ");
			break;

		default :
			return AUTO_FAIL;
	}
	Sleep(5);
	return AUTO_SUCCESS;
}

int mode_cmd(int apcmd)//播放命令
{
	switch(apcmd)
	{
		case SINGLE_PLAY://单曲播放

			break;

		case SINGLE_CIRCULATION ://单曲循环播放

			break;

		case LOOP_PLAY ://循环播放
			break;

		case RANDOM_PLAY  ://随机播放
			break;

		default :
			return AUTO_FAIL;
	}
}

void MusicInit(void)//音乐初始化
{
	int first_init = 1;
	int size;
	char MusicID_buf[100];
	struct dirent *music_dirent;
	DIR *music_dir = opendir(MUSIC_DIR);
	while(NULL != (music_dirent = readdir(music_dir))){//读取文件并去除"."和".."
		if(0 == strcmp(music_dirent->d_name,".") ||
		   0 == strcmp(music_dirent->d_name,".."))
			continue;

		sprintf(MusicID_buf, "%s/%s", MUSIC_DIR, music_dirent->d_name);
		if(first_init){
			music_head = (MusicInfo *)malloc(sizeof(MusicInfo));
			music_head_init(music_head, music_dirent->d_name, MusicID_buf);
			first_init = 0;
		}
		else{/*增加到音乐链表*/
			music_list_add(music_head, music_dirent->d_name, MusicID_buf);
		}
	}
	music_flag = music_head;
	close_dir(music_dir);
}


void MusicDel(MusicInfo *music_node)
{
	MusicInfo *qd;
	music_list_del(music_node);

}

AUTO_API void MusicControl(ModeSelect apcmd)
{
	aplay_cmd(apcmd.MusicCro);
	mode_cmd(apcmd.PlayMode);
}


void* AutoProcess(void *param)
{
	ModeSelect apcmd;
	int client , reclen;
	char recvbuf[100];
	int recvbuf_len = sizeof(recvbuf);

	int serverSocket = TcpServerCreate(CGI_SERVER_PORT, HOST);//服务器socket初始化
	if(serverSocket < 0)
		return NULL;

	while(!auto_mode)
	{
		client = SocketAccept(serverSocket, 3000);//接收客户
		if(client < 0)
			continue;
		memset(recvbuf, 0, recvbuf_len);
		if(SocketRecv(client, recvbuf, recvbuf_len, 1000)){
			reclen = strlen(recvbuf);
			if(1 == reclen){
				apcmd.MusicCro = Atoi(&recvbuf[0]);
				printf("cmd : %d\n",apcmd.MusicCro);
				MusicControl(apcmd);
			}
			/*WEB音乐播放控制*/
		}
	}
}
//多媒体初始化
AUTO_API int auto_init(void)
{
	MusicInit();
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
