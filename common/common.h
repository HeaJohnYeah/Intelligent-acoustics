#ifndef _COMMON__H_
#define _COMMON__H_

#include <stdint.h>

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

#define Sleep(ms) usleep((ms)*1000)

#endif
