#include "common/pthreadpool.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <pthread.h>  
#include <assert.h>  

PoolCreate api_request;

void *myprocess (void *arg)  
{  
    printf ("threadid is 0x%x, working on task %d\n", (unsigned int)pthread_self(),*(int *) arg);  
    sleep (1);/*休息一秒，延长任务的执行时间*/  
    return NULL;  
}  

int main()
{
	Ai_Requset_pool_init(&api_request,10);/*线程池中最多三个活动线程*/  
      
    /*连续向池中投入10个任务*/  
    int *workingnum = (int *) malloc (sizeof (int) * 10);  
    int i;  
    for (i = 0; i < 10; i++)  
    {  
        workingnum[i] = i;  
        Ai_Requset_pool_add(&api_request, myprocess, &workingnum[i]);  
    }  
    /*等待所有任务完成*/  
    sleep (5);  
    /*销毁线程池*/  
    Ai_Requset_pool_destroy(&api_request);  
  
    free (workingnum);  
    return 0;
}