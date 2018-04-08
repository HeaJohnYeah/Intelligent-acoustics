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
#include <errno.h>
#include <netdb.h>

#include "sndfile.h"

#include "../common/msr.h"
#include "../common/auto.h"
#include "../common/socket.h"
#include "../common/common.h"
#include "../common/pthreadpool.h"


PoolCreate api_request;//�̳߳�

//���ø�ʽ
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

//�ļ�����
const MSR_U8 *FILE_TYPE[4]={
		"voc","wav","raw","au"
};

/*---------------SND  WAV  FILE------------------*/

static double waves_max(double *buf)//����������
{
	double max;
	int arraylenth =sizeof(buf);
	int i;
	for(i = 0; i < arraylenth; i++)
		if(fabs(buf[i] - max) >= ESP)
				max = buf[i];
	return max;
}

//��ȡwav�ļ���Ϣ,����wav����ж�
static int snd_read_wav(char *wav_path, double wav_flag)
{
	SNDFILE *sf;
	SF_INFO info;
	int num_channels;
	int num_items;
	int num_wav;
	int ret;
	double *wavbuf;

	info.format = 0;

	sf = sf_open(wav_path,SFM_READ,&info);//��ȡwav��Ϣ
	if(NULL == sf){
		printf("Failed to open the file.\n");
		sf_close(sf);
		return WAV_FAIL;
	}

	printf("frames=%d\n samplerate=%d\n channels=%d\n", (int)info.frames, (int)info.samplerate, (int)info.channels);

	num_items = info.frames * info.channels;
	printf("num_items=%d\n",num_items);

	wavbuf = (double *)malloc(num_items*sizeof(double));
	if(NULL == wavbuf){
		printf("Create wav buf failed.\n");
		sf_close(sf);
		return WAV_FAIL;
	}

    num_wav = sf_read_double(sf,wavbuf,num_items);//��ȡ��������
    sf_close(sf);

	double Mwav = waves_max(wavbuf);//�����������
	if(fabs(Mwav) - wav_flag)
		ret = WAV_SUCCESS;
	else
		ret = WAV_FAIL;
	free(wavbuf);

	return ret;
}


/*------------------------------------------------------*/

//¼�����������ʼ��
static int res_info_init(RecordInfo *rest,	MSR_U8 *rate, MSR_U8 *duration, MSR_U8 *channels, MSR_U8 *format, MSR_U8 *type)
{
	rest = (RecordInfo *)malloc(sizeof(RecordInfo));
	if(NULL == rest){
		printf("request malloc fail\n");
		return -1;
	}
	rest->rate = rate;
	rest->duration = duration;
	rest->channels = channels;
	rest->format = format;
	rest->type = type;
	return 0;
}

//¼����������
static void record_cmd(RecordInfo *rest_info, MSR_U8 *cmd)
{
	sprintf(cmd, "arecord -c %s -d %s -f %s -r %s -t %s record.%s",
				rest_info->channels, rest_info->duration,
				rest_info->format,rest_info->rate,
				rest_info->type,rest_info->type);
}

//¼�������߳�
static void *request_process(void *arg)
{
	RecordInfo *rest_info = (RecordInfo *)arg;
	MSR_U8 *recmd = malloc(CMD_SIZE);
	double wav_flag = 0;
	record_cmd(rest_info, recmd);
	if(NULL == recmd)
		printf("record cmd fail\n");

	while(1)
	{
		system(recmd);
		int ret = snd_read_wav(WAV_PATH, wav_flag);
		if(ret == WAV_FAIL || ret == WAV_CONTINUE){
		  	Sleep(5);
			continue;
		}


	}
}

MSR_API int msr_init(int task_num)
{
	Ai_Requset_pool_init(&api_request,task_num);	//�����߳���¼��

	RecordInfo *rest_info[task_num];//¼������

	MSR_U8 *rate 	 = "16000";//Ƶ��
	MSR_U8 *duration = "4";//����ʱ�䣬��Ϊ��λ
	MSR_U8 *channels = "2";//ͨ����
	MSR_U8 *format 	 = (MSR_U8 *)FORMAT[4];
	MSR_U8 *type 	 = (MSR_U8 *)FILE_TYPE[1];

	int i,ret;
    for (i = 0; i < task_num; i++) {
       	ret = res_info_init(rest_info[i], rate, duration, channels, format, type);
		if(0 != ret){
			printf("request info init fail task number is %d\n",i);
			goto msr_destroy;
		}
        ret = Ai_Requset_pool_add(&api_request, request_process, rest_info[i]);
		if(0 != ret){
			printf("create request fail task number is %d\n",i);
			goto msr_destroy;
		}
    }

	return 0;

msr_destroy :
	msr_fini();
	free(rest_info);
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