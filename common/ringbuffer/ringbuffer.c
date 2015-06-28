
#define RINGBUFFER_LOG	1
//#define DBUG_RBUF(x) do{\if( RINGBUFFER_LOG )serial_printf x;}while(0) 


 
#include "uart2.h"
#include "ringbuffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DRV_DEBUG 0
#if RINGBUFFER_LOG
    #define DBUG_RBUF(fmt, ...) //serial_printf(fmt,## __VA_ARGS__)
#else
    #define DBUG_RBUF(fmt, ...) 
#endif

#ifdef LIUNX
#include <thread.h>

#else

int rbuf_mutex_init( rbuf_mutex_t *mute_flag ,char* val) 
{
	*mute_flag = 0;//*(rbuf_mutex_t *)val ;	 
	return 0;
}

void rbuf_mutex_lock(rbuf_mutex_t *mute_flag )
{
	do{
	}while( *mute_flag ) ;
	*mute_flag = 1;
}

void rbuf_mutex_unlock( rbuf_mutex_t *mute_flag )
{
	*mute_flag = 0;
}
#endif
 
/**
 **  rbuf    ringbuf 
 **  len     ringbuf length
 **  return  -1 false  0 true 
**/
ssize_t ringbuffer_init(P_ST_RINGBUFFER rbuf,size_t len)
{
	memset(rbuf, 0 , sizeof(ST_RINGBUFFER) );
	if( rbuf_mutex_init(&rbuf->mutex, NULL) != 0)
	{
		//DBUG_RBUF("rbuf_mutex_init error \n");
		return 0 ;
	}
	rbuf->read = rbuf->write = 0;
	rbuf->data = (unsigned char *)malloc(len);
	if( rbuf->data == NULL)
	{
		//DBUG_RBUF(" malloc error for rubuffer data \n");
		rbuf->size = 0 ;
		rbuf->error = 1; 
		return 0 ;
	}
	else
	{
		rbuf->size = len;
		rbuf->error = 0;
	}
	return 1 ;
}

/**
 **  rbuf    ringbuf 
 **  
 **  return  get ringbuffer free buff size 
**/
ssize_t ringbuffer_free(P_ST_RINGBUFFER rbuf)
{
//	DBUG_RBUF("@ringbuffer_free, write : %d, read : %d \n", rbuf->write, rbuf->read) ;
	ssize_t free;

	rbuf_mutex_lock( &rbuf->mutex);
	free = rbuf->read - rbuf->write;
	if( free <= 0)
	{
		free += rbuf->size;
	}
	rbuf_mutex_unlock(&rbuf->mutex);
	//DBUG_RBUF("@ringbuffer_free, ringbuffer free is %d", free - 1);
	return free - 1;
}

/**
 **  rbuf    ringbuf 
 **  
 **  return  get ringbuffer free buff size 
**/
ssize_t ringbuffer_avail(P_ST_RINGBUFFER rbuf)
{
    long int avail;
    DBUG_RBUF("@ringbuffer_avail, write : %d, read : %d \n", rbuf->write, rbuf->read);
    rbuf_mutex_lock(&(rbuf->mutex));
    avail = rbuf->write - rbuf->read;
    if (avail < 0)
    {
        avail += rbuf->size;
    }
    rbuf_mutex_unlock(&(rbuf->mutex));

    DBUG_RBUF("@ringbuffer_avail, avail is %d \n", avail);

    return avail;
}

/**
 **  rbuf    ringbuf 
 **  
 **  return  get ringbuffer reset 
**/
void ringbuffer_reset(P_ST_RINGBUFFER rbuf)
{
	rbuf_mutex_lock(&rbuf->mutex);
	rbuf->read = rbuf->write = 0;
	rbuf_mutex_unlock(&rbuf->mutex);
}

/**
 **  rbuf    ringbuf 
 **  dat     ringbuf first byte
 **  return  get  one byte from ringbuffer
**/
ssize_t ringbuffer_read_byte( P_ST_RINGBUFFER rbuf ,unsigned char *dat)
{
	unsigned char ret,avail;
	if (rbuf == NULL)
	{
		DBUG_RBUF("@ringbuffer_read, buf is NULL!\n");
		return 0;
	}
	if (rbuf->error)
	{
		DBUG_RBUF("@ringbuffer_read, rbuf->error : %d\n", rbuf->error);
		return 0;
	}
	avail = ringbuffer_avail(rbuf);
	rbuf_mutex_lock(&rbuf->mutex);
	if( avail != 0 )
	{
		*dat = rbuf->data[rbuf->read];
		rbuf->read  = ( rbuf->read == rbuf->size ) ? 0 : (rbuf->read +1);
		ret = 1	 ;
		
	}else{
		DBUG_RBUF("@ringbuffer_avail no date \n");
		ret = 0;
	}
	rbuf_mutex_unlock(&rbuf->mutex);
	return ret ;
}
ssize_t ringbuffer_write_byte( P_ST_RINGBUFFER rbuf ,const unsigned char w_byte)
{
    long int free_size ;
	if (rbuf == NULL)
	{
		DBUG_RBUF("@ringbuffer_read, buf is NULL!\n");
		return 0;
	}
	if (rbuf->error)
	{
		DBUG_RBUF("@ringbuffer_read, rbuf->error : %d\n", rbuf->error);
		return 0;
	}

    free_size = ringbuffer_free(rbuf);
	rbuf_mutex_lock(&rbuf->mutex);
    if( free_size != 0 )
    {
        rbuf->data[rbuf->write] = w_byte;
        rbuf->write = ( rbuf->write == rbuf->size ) ? 0 : (rbuf->write + 1);
    }   
	rbuf_mutex_unlock(&rbuf->mutex);
	return 1;
}

ssize_t ringbuffer_read(P_ST_RINGBUFFER rbuf, unsigned char *buf, size_t len)
{
	ssize_t avail ;
	ssize_t split ;
	ssize_t todo = len;
	if (buf == NULL)
	{
		DBUG_RBUF("@ringbuffer_read, buf is NULL!\n");
		return 0;
	}
	if (rbuf->error)
	{
		DBUG_RBUF("@ringbuffer_read, rbuf->error : %d\n", rbuf->error);
		return 0;
	}
	avail = ringbuffer_avail(rbuf);
	if( avail > len )
	{
		avail = len ;
	}
	else if( avail > 0 && avail < len)
	{
		todo = avail;
	}
	else
	{
		return 0 ;
	}
	rbuf_mutex_lock(&rbuf->mutex);
	split = (rbuf->read + avail > rbuf->size ) ? rbuf->size - rbuf->read : 0 ;
	if( split > 0 )
	{
		memcpy( buf , rbuf->data + rbuf->read , split);
		rbuf->read = 0;
		buf += split ;
		todo -= split ;
	}
	memcpy( buf , rbuf->data + rbuf->read , todo);
	rbuf->read = (rbuf->read + todo) % rbuf->size;
	rbuf_mutex_unlock(&rbuf->mutex);
	return avail;
}

ssize_t ringbuffer_write(P_ST_RINGBUFFER rbuf, const unsigned char *buf, size_t len)
{
    size_t todo = len;
    size_t split;
    size_t free_size;

    if (buf == NULL || len == 0)
    {
        DBUG_RBUF("@ringbuffer_write, buf is empty!\n");
        return 0;
    }

    free_size = ringbuffer_free(rbuf);
    DBUG_RBUF("@ringbuffer_write, rbuf->write : %d, rbuf->read : %d, free_size : %d\n", rbuf->write, rbuf->read, free_size);
    if (free_size >= len)
    {
        free_size = len;
    }
    else
    {
        DBUG_RBUF("@ringbuffer_write, buffer is full!!");
//        usleep(20 * 1000);
        return 0;
    }

    rbuf_mutex_lock(&(rbuf->mutex));
    split = (rbuf->write + free_size > rbuf->size) ? rbuf->size - rbuf->write : 0;
    if (split > 0)
    {
        memcpy(rbuf->data + rbuf->write, buf, split);
        buf += split;
        todo -= split;
        rbuf->write = 0;
        DBUG_RBUF("@ringbuffer_write, end of the buffer.\n");
    }
    memcpy(rbuf->data + rbuf->write, buf, todo);
    rbuf->write = (rbuf->write + todo) % rbuf->size;
    rbuf_mutex_unlock(&(rbuf->mutex));

    return free_size;
}


