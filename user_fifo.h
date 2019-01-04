#ifndef USER_FIFO_H_
#define USER_FIFO_H_

#include "unix_fifo_ops.h"

unixFifoOps_t * user_fifo_write_init(const char *path);

#endif