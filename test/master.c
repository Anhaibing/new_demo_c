#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../fifo/user_fifo.h"
#include "../msgQueue/user_msgqueue.h"

int msg_queue_test(){
    int msgId = create_MsgQueue();
    char buf[1024] = {0};
    while(1){
        recv_MsgQueue(msgId, CLIENT_TYPE, buf);
        if(strcasecmp("quit", buf) == 0)
            break;
        inf("client say: %s", buf);
        inf("Please insert what you want to say");
        fflush(stdout);
        ssize_t count = read(0, buf, sizeof(buf));
        if(count > 0){
            buf[count-1] = 0;
            send_MsgQueue(msgId, SERVER_TYPE, buf);
            inf("send done,wait recvive...");
        }
    }
	destory_MsgQueue(msgId);
    return 0;
}

int main(int argc, char * argv[]) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "l::p::gtwevh")) != -1) {
        switch (opt) {
            case 'l':
                log_init(optarg, NULL); //log level
                break;
            case 'p':
                log_init(NULL, optarg); //log file
                break;
            case 'v':
                printf("get opt is %c", opt);
                break;
            case 'h':
                printf("get opt is %c", opt);
                break;
            default:
                return -1;
        }
    }

	msg_queue_test();
	return 0;

	unixFifoOps_t *fifoPtr = user_fifo_write_init("anbinFIFO");
	if(!fifoPtr) {
		err("user_fifo_write_init failed");
		return -1;
	}

	int i = 0;
	for(i = 0; i < 10; i++){
		user_fifo_write_fmt(fifoPtr,"anbintest %d", i);	//for test
		sleep(1);
	}

    return 0;
}
