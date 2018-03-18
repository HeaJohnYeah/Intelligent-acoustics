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
	printf("%s %d\n",__func__,__LINE__);\
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ save close !!!!!!!!!!!!!!!!!!!!!!111\n");\
	fflush(fp);\
	fsync(fileno(fp));\
	fclose(fp);\
	system("sync");\
	usleep(10000);\
}

//不带打印的同步
#define fclose_save_nopr(fp) { \
	fflush(fp);\
	fsync(fileno(fp));\
	fclose(fp);\
	system("sync");\
	usleep(10000);\
}


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


