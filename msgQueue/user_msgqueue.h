#ifndef _USER_MSGQUEUE_H_
#define _USER_MSGQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include "../log/slog.h"

typedef struct msg_buf_t{
    long msgtype;
    char msgtext[1024];
}msg_buf_t;

#define SERVER_TYPE 1
#define CLIENT_TYPE 2

int create_MsgQueue();
int get_MsgQueue();
int send_MsgQueue(int msgId, int sender, char * msg);
int recv_MsgQueue(int nsgId, int recvtype, char * out);
int destory_MsgQueue(int msgId);

#endif
