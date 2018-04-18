#ifndef _AIPYTHON__H_
#define _AIPYTHON__H_

#include <stdint.h>

#define AIPYTHON_API

#define AI_SOUND_MOD  "AIsound"
#define SOUND_FUN 	  "bai_request"


#ifdef __cplusplus
extern "C" {
#endif


AIPYTHON_API int BaiduRequest(char *module, char *func, char *arg);



#ifdef __cplusplus
}
#endif


#endif


