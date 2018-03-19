#ifndef _AUTO__H_
#define _AUTO__H_

#include <stdint.h>

#ifdef _WIN32
#ifdef AUTO_EXPORTS
#define AUTO_API __declspec(dllexport)
#else
#define AUTO_API __declspec(dllimport)
#endif
#else
#define AUTO_API
#endif

#define fclose_save(fp) { \
	fflush(fp);\
	fsync(fileno(fp));\
	fclose(fp);\
	system("sync");\
	usleep(10000);\
}

#define close_dir(fp){ \
	closedir(fp); \
	fp = NULL; \
	usleep(10000); \
}

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


