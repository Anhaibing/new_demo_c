#include "unix_fifo_ops.h"

int fd_set_flag(int fd,int flag){
	int flags = fcntl(fd,F_GETFL, 0);
	if(flags<0){
		show_errno(0,"fcntl F_GETFL failed!");
		return -1;
	}
	int ret=fcntl(fd,F_SETFL, flags|flag);
	if(ret<0){
		show_errno(0,"fcntl F_SETFL failed!");
		return -2;		
	}
	return 0;
}


unixFifoOps_t * unix_fifo_ops_create(const char * path, char needlock){
	unixFifoOps_t * ptr = calloc(1, sizeof(unixFifoOps_t));
	if(!ptr) {
		err("calloc failed");
		return NULL;
	}
	ptr->path = path;
	ptr->needlock = needlock;
	if(needlock)
		pthread_mutex_init(&ptr->mtx, NULL);
	ptr->fd = unix_fifo_ops_init(ptr);
	if(ptr->fd < 0){
		err("unix_fifo_ops_init failed");
		return NULL;
		//need free???
	}
	return ptr;
}

int unix_fifo_ops_init(unixFifoOps_t* ptr){
	if(ptr->needlock)
		pthread_mutex_lock(&ptr->mtx);
	int fd = un_open(ptr->path, O_RDWR|O_EXCL, 0666);
	if (fd < 0){
		mkfifo(ptr->path, 06666);
		fd = un_open(ptr->path, O_RDWR|O_EXCL, 0666);
		if(fd < 0){
			err("open %s failed", ptr->path);
			return fd;
		}
		inf("open %s succeed!", ptr->path);
	}
	if(ptr->needlock)
		pthread_mutex_unlock(&ptr->mtx);
	return fd;
}

