#ifndef _AUTO__H_
#define _AUTO__H_

#include <stdint.h>

#define AUTO_API

#define MUSIC_DIR "../music"


typedef signed char             AUT_S8;
typedef signed short            AUT_S16;
typedef signed int              AUT_S32;
typedef signed long long        AUT_S64;

typedef unsigned char           AUT_U8;
typedef unsigned short          AUT_U16;
typedef unsigned int            AUT_U32;
typedef unsigned long long      AUT_U64;


#ifdef __cplusplus
extern "C" {
#endif


//函数添加
AUTO_API int  auto_init();
AUTO_API void auto_fini();

#ifdef __cplusplus
}
#endif


#endif


