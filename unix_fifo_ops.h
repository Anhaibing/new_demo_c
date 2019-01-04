#ifndef UNIX_FIFO_OPS_H_
#define UNIX_FIFO_OPS_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <limits.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "slog.h"

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

int fd_set_flag(int fd,int flag);

unixFifoOps_t * unix_fifo_ops_create(const char *path, char needlock);
int unix_fifo_ops_init(unixFifoOps_t* ptr);


#endif
