#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif  /* End of #ifdef __cplusplus */

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <pthread.h>  
#include <assert.h>  
#include "../common/pthreadpool.h"

PTHREAD_POOL int Ai_Requset_pool_add(PoolCreate *AiResTask, void *(*process)(void *arg), void *arg)
{
	    /*构造一个新任务*/  
    PoolTask *newtask = (PoolTask *) malloc (sizeof (PoolTask));  
    newtask->process = process;  
    newtask->arg = arg;  
    newtask->next = NULL;/*别忘置空*/  
  
    pthread_mutex_lock (&(AiResTask->mutex));  
    /*将任务加入到等待队列中*/  
    PoolTask *member = AiResTask->api_request_head;  
    if (member != NULL)  
    {  
        while (member->next != NULL)  
            member = member->next;  
        member->next = newtask;  
    }  
    else  
    {  
        AiResTask->api_request_head = newtask;  
    }   
  
    AiResTask->cur_request_size++;  
    pthread_mutex_unlock (&(AiResTask->mutex));  
 
    //注意如果所有线程都在忙碌，这句没有任何作用 
    pthread_cond_signal(&(AiResTask->cond));  
    return 0;  
}


PTHREAD_POOL void *thread_routine (void *arg)
{ 
	PoolCreate *msg = (PoolCreate *)arg;
    while (1)  
    {  
        pthread_mutex_lock (&(msg->mutex));  
        //如果等待队列为0并且不销毁线程池，则处于阻塞状态; 注意 
        //pthread_cond_wait是一个原子操作，等待前会解锁，唤醒后会加锁  
        while (0 == msg->cur_request_size && 0 == msg->shutdown)  
        {  
			printf ("thread 0x%x is waiting\n", (unsigned int)pthread_self());  
            pthread_cond_wait (&(msg->cond), &(msg->mutex));  
        }  
  
        /*线程池要销毁了*/  
        if (msg->shutdown)  
        {  
            /*遇到break,continue,return等跳转语句，千万不要忘记先解锁*/  
            pthread_mutex_unlock (&(msg->mutex));  
			printf ("thread 0x%x will exit\n", (unsigned int)pthread_self());  
            pthread_exit (NULL);  
        }  
  
        printf ("thread 0x%x is starting to work\n", (unsigned int)pthread_self());  
      
        /*等待队列长度减去1，并取出链表中的头元素*/  
        msg->cur_request_size--;  
        PoolTask *task = msg->api_request_head;  
        msg->api_request_head = task->next;  
        pthread_mutex_unlock (&(msg->mutex));  
  
        /*调用回调函数，执行任务*/  
        (*(task->process))(task->arg);  
        free (task);  
        task = NULL;  
    }  
    /*这一句应该是不可达的*/  
    pthread_exit (NULL);  
}

PTHREAD_POOL int Ai_Requset_pool_init(PoolCreate *AiResTask, int max_request_num)
{

	//初始化线程池结构体
	AiResTask = (PoolCreate *) malloc (sizeof (PoolCreate)); 
	AiResTask->pthreadID = (pthread_t *)malloc(max_request_num*sizeof(pthread_t));	
	
    pthread_mutex_init (&(AiResTask->mutex), NULL);  
    pthread_cond_init  (&(AiResTask->cond), NULL); 
	
	
	AiResTask->cur_request_size = 0;
	AiResTask->max_request_num = max_request_num;
	AiResTask->shutdown = 0;
	
	AiResTask->api_request_head = NULL;
	
	int i = 0;
	for(i = 0; i < max_request_num; i++)
	{
		pthread_create(&(AiResTask->pthreadID[i]), NULL, thread_routine, (void *)AiResTask);	
	}
	return 0;
}

PTHREAD_POOL int Ai_Requset_pool_destroy(PoolCreate *AiResTask)
{
	if (AiResTask->shutdown)  
       return -1;//防止两次调用
	AiResTask->shutdown = 1;
	/*唤醒所有等待线程，线程池要销毁了*/  
    pthread_cond_broadcast (&(AiResTask->cond));
	/*阻塞等待线程退出，否则就成僵尸了*/  
    int i;  
    for(i = 0; i < AiResTask->max_request_num; i++)  
        pthread_join(AiResTask->pthreadID[i], NULL);  
    free(AiResTask->pthreadID); 
	
	/*销毁等待链表*/  
    PoolTask *head = NULL;  
    while (AiResTask->api_request_head != NULL)  
    {  
        head = AiResTask->api_request_head;  
        AiResTask->api_request_head = AiResTask->api_request_head->next;  
        free (head);  
    } 
	return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */