#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#ifdef _LINUX_
#include <thread.h>
#include <sys/types.h>
typedef	thread_mutex_t rbuf_mutex_t ;
typedef	pthread_mutex_init rbuf_mutex_init;
typedef	pthread_mutex_unlock rbuf_mutex_unlock ;
typedef	pthread_mutex_lock rbuf_mutex_lock ;
#else
typedef unsigned int size_t;
typedef unsigned int ssize_t;
typedef int rbuf_mutex_t ;

#endif

typedef struct
{
	unsigned char *data;
	unsigned long int size;
	unsigned long int read;
	unsigned long int write;
	int error;
	rbuf_mutex_t mutex;
}ST_RINGBUFFER,*P_ST_RINGBUFFER;

ssize_t ringbuffer_init(P_ST_RINGBUFFER rbuf, size_t len);
ssize_t ringbuffer_free(P_ST_RINGBUFFER rbuf);
ssize_t ringbuffer_avail(P_ST_RINGBUFFER rbuf);
ssize_t ringbuffer_read(P_ST_RINGBUFFER rbuf, unsigned char *buf, size_t len);
size_t ringbuffer_write(P_ST_RINGBUFFER rbuf, const unsigned char *buf, size_t len);
void ringbuffer_reset(P_ST_RINGBUFFER rbuf);
void ringbuffer_destroy(P_ST_RINGBUFFER rbuf);
ssize_t ringbuffer_write_byte( P_ST_RINGBUFFER rbuf ,const unsigned char w_byte) ;
ssize_t ringbuffer_read_byte( P_ST_RINGBUFFER rbuf , unsigned char *r_byte);
#endif   /*  _RINGBUFFER_H */
