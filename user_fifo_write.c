#include "user_fifo.h"

unixFifoOps_t * user_fifo_write_init(const char * path) {
	unixFifoOps_t * ptr = unix_fifo_ops_create(path, 0);
	if(!ptr){
		err("ptr create failed!");
		return NULL;
	}
	int ret = fd_set_flag(ptr->fd, O_NONBLOCK);
	if(ret < 0){
		err("fd_set_flag failed!");
		return NULL;
	}
	return ptr;
}

