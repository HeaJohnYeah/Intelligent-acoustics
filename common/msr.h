#ifndef _MSR__H_
#define _MSR__H_

#include <stdint.h>

#define MSR_API

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


#ifdef __cplusplus
extern "C" {
#endif


//函数添加


#ifdef __cplusplus
}
#endif


#endif