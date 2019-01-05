#ifndef UNIX_FIFO_OPS_H_
#define UNIX_FIFO_OPS_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "../log/slog.h"

typedef struct unixFifoOps_t{
    int fd;
    char *path;
    char needlock;
    pthread_mutex_t mtx;
}unixFifoOps_t;

#define un_open(path,flags,mode) ({\
	int fd=0;\
	fd=open(path,flags,mode);\
	if(-1==fd){\
		show_errno(0,"open!");\
	}\
	fd;\
})

#define un_write(fd,buf,size) ({\
	ssize_t ret=0,count=0;\
	for(;;){\
		ret=write(fd,buf+count,size-count);\
		if(ret>0)count+=ret;\
		if(count==size){\
			break;\
		}\
		if(errno==EINTR){\
			show_errno(0,"write");\
			continue;\
		}\
		if(ret<0 && errno!=EAGAIN){\
			show_errno(0,"write");\
		}\
		break;\
	}\
	count;\
})

int fd_set_flag(int fd,int flag);

unixFifoOps_t * unix_fifo_ops_create(const char *path, char needlock);
int unix_fifo_ops_init(unixFifoOps_t* ptr);
ssize_t unix_fifo_ops_write(unixFifoOps_t* ptr,char *buf,size_t size);

#endif
