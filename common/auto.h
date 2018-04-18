#ifndef _AUTO__H_
#define _AUTO__H_

#include <stdint.h>

#define AUTO_API

#define AUTO_FAIL      (-1)
#define AUTO_SUCCESS   ( 0)

#define MUSIC_DIR "../music"

#define PAUSE(id)  kill(id, SIGSTOP)
#define REPLAY(id)   kill(id, SIGCONT)
#define STOP(id)  kill(id, SIGINT)

typedef signed char             AUT_S8;
typedef signed short            AUT_S16;
typedef signed int              AUT_S32;
typedef signed long long        AUT_S64;

typedef unsigned char           AUT_U8;
typedef unsigned short          AUT_U16;
typedef unsigned int            AUT_U32;
typedef unsigned long long      AUT_U64;

enum MUSIC_CRO{
	MUSIC_PAUSE = 0,
 	MUSIC_PLAY,
    MUSIC_NEXT,
    MUSIC_PREV,
    MUSIC_STOP
};

enum PLAY_MODE{
	SINGLE_PLAY = 0,
 	SINGLE_CIRCULATION,
    LOOP_PLAY,
    RANDOM_PLAY
};

typedef struct {
	int  MusicCro;
	int  PlayMode;
}ModeSelect;



#ifdef __cplusplus
extern "C" {
#endif

//函数添加
AUTO_API void MusicControl(ModeSelect apcmd);


AUTO_API int  auto_init(void);
AUTO_API void auto_fini(void);

#ifdef __cplusplus
}
#endif


#endif


