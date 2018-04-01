#ifndef _PTHREADPOOL__H_
#define _PTHREADPOOL__H_

#include <stdint.h>
#include <pthread.h>  
#define PTHREAD_POOL

/* 
 *线程池里所有运行和等待的任务都是一个PoolTask 
 *由于所有任务都在链表里，所以是一个链表结构 
 */  

typedef struct task{
	//回调函数，任务运行时会调用此函数，注意也可声明成其它形式  
	void 	 *(*process)(void *arg);
	void 	 *arg;//回调函数的参数
	struct   task *next;
} PoolTask;


/*线程池结构*/  
typedef struct pool{  
	pthread_t *pthreadID; 
    pthread_mutex_t mutex;  
    pthread_cond_t 	cond;  	
    //链表结构，线程池中所有等待任务  
    PoolTask *api_request_head;    
    //线程池中允许的活动线程数目 
    int max_request_num;  
    //当前等待队列的任务数目  
    int cur_request_size;  
	//是否销毁线程池 
    int shutdown; 
} PoolCreate;  

#ifdef __cplusplus
extern "C" {
#endif


//函数添加
PTHREAD_POOL int  Ai_Requset_pool_init(PoolCreate *AiResTask, int max_request_num);
PTHREAD_POOL int  Ai_Requset_pool_destroy(PoolCreate *AiResTask);
PTHREAD_POOL int  Ai_Requset_pool_add(PoolCreate *AiResTask, void *(*process)(void *arg), void *arg);


#ifdef __cplusplus
}
#endif


#endif