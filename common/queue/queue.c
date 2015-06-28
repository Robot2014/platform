#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#ifdef _LINUX_
#include <thread.h>
#include <sys/types.h>

#define	pthread_mutex_t queue_mutex_t 
#define	pthread_mutex_init queue_mutex_init
#define	pthread_mutex_unlock queue_mutex_unlock 
#define	pthread_mutex_lock queue_mutex_lock 
#define pthread_mutex_destory queue_mutex_destory 
#else 
typedef int queue_mutex_t ;
static int queue_mutex_init( queue_mutex_t* qmutex , char *str)
{
    *qmutex = 0 ;
    return 0 ;
}
static int queue_mutex_unlock( queue_mutex_t* qmutex )
{
    *qmutex = 0 ;
    return 0 ;   
}
static int queue_mutex_lock( queue_mutex_t* qmutex )
{
    do{
    }while(*qmutex) ;
    *qmutex = 1 ;
    return 0 ;
}
static int queue_mutex_destory( queue_mutex_t* qmutex )
{
    *qmutex = 0 ;
    return 0 ;   
}
#endif 

P_QUEUE  queue_init( ST_QUEUE *queue )
{
	if( queue == NULL )
	{
		queue->error = 1 ;
	}else 
	{
		queue->error = 0;
	}
	queue->head = NULL ;
	queue->tail = NULL ;
	queue->size = 0 ;
	queue_mutex_init(&queue->qmutex,NULL ) ;
	return queue;
}
uint8_t queue_size(P_QUEUE queue)
{
    uint8_t  q_size ;
    queue_mutex_lock(&queue->qmutex) ;
    q_size = queue->size ;
    queue_mutex_unlock(&queue->qmutex) ;
    return q_size ;
}

QUEUE_STATUS queue_empty(P_QUEUE queue)
{
	return queue->size == 0 ? QUEUE_EMPTY : QUEUE_FULL ;
}

QUEUE_STATUS queue_push(P_QUEUE queue, ELEMENT_TYPE node )
{
	P_QUEUE_NODE new_node ;
	if( queue == NULL || node == NULL || queue->error == 1 )
	{
	    return QUEUE_NONE;
	}
    if( queue->size > QUEUE_MAX_SIZE )
        return QUEUE_FULL ;
	queue_mutex_lock( &queue->qmutex ) ;
	new_node = malloc(sizeof( ST_QUEUE_NODE )) ;
	new_node->data = node ;
	new_node->next = NULL ;
	if( queue->head == NULL )
	{
	    queue->head = new_node ;
	}
    else
	{
	    queue->tail->next = new_node ;
	}
    queue->tail = new_node ;
	queue->size++;
	queue_mutex_unlock(&queue->qmutex) ;
	return QUEUE_SUCCESS;
}

ELEMENT_TYPE queue_pop(P_QUEUE queue )
{
	ST_QUEUE_NODE qhead ;
    
	if( queue == NULL || queue->error == 1 )
	{
		return QUEUE_NONE;
	}
	if(queue->size == 0 )
	{
		return QUEUE_EMPTY;
	}
	queue_mutex_lock( &queue->qmutex ) ;
	qhead = *(queue->head) ;
    free( queue->head );
	queue->head = qhead.next ;
	queue->size-- ;
	queue_mutex_unlock( &queue->qmutex );
	return qhead.data;
}

void queue_clear(P_QUEUE queue)
{
    P_QUEUE_NODE qhead = queue->head;
    queue_mutex_lock(&queue->qmutex) ;
    while( qhead != NULL )
    {
        queue->head = queue->head->next ;
        free(qhead);
        qhead = queue->head ;
    }
    queue->tail = NULL ;
    queue_mutex_unlock(&queue->qmutex) ;
    return ;
}

void queue_destory(P_QUEUE queue )
{
    queue_clear(queue ) ;
	queue_mutex_destory(&queue->qmutex) ;
}