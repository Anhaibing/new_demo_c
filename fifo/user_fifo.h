#ifndef USER_FIFO_H_
#define USER_FIFO_H_

#include "unix_fifo_ops.h"

//used for fifo cmd parse
#define FMT_PREFIX	"^"
#define FMT_SUBFIX	"\n"
#define FMT_PREFIX_LEN	(sizeof(FMT_PREFIX)-1)
#define FMT_SUBFIX_LEN	(sizeof(FMT_SUBFIX)-1)

unixFifoOps_t * user_fifo_write_init(const char *path);
int user_fifo_write_fmt(unixFifoOps_t *ptr,const char *fmt,...);
int user_fifo_write_str(unixFifoOps_t *ptr,char *contex);

#endif