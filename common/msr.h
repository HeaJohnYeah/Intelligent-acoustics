#ifndef _MSR__H_
#define _MSR__H_

#include <stdint.h>

#define MSR_API
#define CMD_SIZE        (60)

typedef signed char             MSR_S8;
typedef signed short            MSR_S16;
typedef signed int              MSR_S32;
typedef signed long long        MSR_S64;

typedef unsigned char           MSR_U8;
typedef unsigned short          MSR_U16;
typedef unsigned int            MSR_U32;
typedef unsigned long long      MSR_U64;

/******************************
*	arecord [flags] [filename]
*
*	-r, --rate=#<Hz>
*		设置频率.
*	-d, --duration=#
*	  	设置持续时间,单位为秒.
*	-c, --channels=#
*  	    设置通道号.
*	-f --format=FORMAT
*		设置格式.格式包括: S8          U8  S16_LE  S16_BE  U16_LE
*		U16_BE  S24_LE S24_BE U24_LE U24_BE S32_LE S32_BE U32_LE U32_BE
*		FLOAT_LE  FLOAT_BE  FLOAT64_LE  FLOAT64_BE   IEC958_SUBFRAME_LE
*		IEC958_SUBFRAME_BE MU_LAW A_LAW IMA_ADPCM MPEG GSM
*	-t, --file-type TYPE
*		文件类型(voc,wav,raw或au).
*********************************/

typedef struct{
	MSR_U8 *rate;
	MSR_U8 *duration;
	MSR_U8 *channels;
	MSR_U8 *format;
	MSR_U8 *type;
} RecordInfo;


#ifdef __cplusplus
extern "C" {
#endif


//函数添加
MSR_API int msr_init(int task_num);
MSR_API void msr_fini();

#ifdef __cplusplus
}
#endif


#endif
