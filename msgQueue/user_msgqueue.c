#include "user_msgqueue.h"

int init_MsgQueue(int flags){
    key_t msgKey = ftok("./", 6666);
    if(msgKey < 0) {
        perror("ftok");
        return -1;
    }

    int msgId = msgget(msgKey, flags);
    if(msgId < 0){
        perror("msgget");
        return -1;
    }

    return msgId;
}

int create_MsgQueue(){
    return init_MsgQueue(IPC_CREAT | IPC_EXCL | 0666);
}

int get_MsgQueue(){
    return init_MsgQueue(IPC_CREAT);
}

int send_MsgQueue(int msgId, int sender, char * msg){
    msg_buf_t msgbuf;
    msgbuf.msgtype = sender;
    strcpy(msgbuf.msgtext, msg);
    if(msgsnd(msgId, (void*)&msgbuf, sizeof(msgbuf.msgtext), 0) < 0){
        perror("msgsnd");
        return -1;
    }

    return 0;
}

int recv_MsgQueue(int msgId, int recvtype, char * out){
    msg_buf_t msgbuf;
    int size = sizeof(msgbuf.msgtext);
    if(msgrcv(msgId, (void*)&msgbuf, size, recvtype, 0) < 0){
        perror("msgrcv");
        return -1;
    }

    strncpy(out, msgbuf.msgtext, size);

    return 0;
}

int destory_MsgQueue(int msgId){
    if(msgctl(msgId, IPC_RMID, NULL) < 0){
        perror("msgctl");
        return -1;
    }

    return 0;
}
