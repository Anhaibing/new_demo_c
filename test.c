#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "./fifo/user_fifo.h"

static int anbintest_handle(item_arg_t *arg){
	char **argv=arg->argv;
	if(!argv[1])
		return -1;
	inf("slave: I get data %d", atoi(argv[1]));
	return 0;
}

cmd_item_t fifo_cmd_tbl[]={
	ADD_HANDLE_ITEM(anbintest,NULL)
	{0}//表轮询结束项（不加此项可能会导致程序异常
};

void * user_fifo_proc(){
	inf("%s", __func__);
	fifo_cmd_t fifo_cmd_args={
		.type=FIFO_CMD_UNSPECIFIED_FIFO,
		.path="anbinFIFO",	//same as master
		.tbl=fifo_cmd_tbl

	};
	int ret=user_fifo_read_init(&fifo_cmd_args);
	if(ret<0) {
		err("user_fifo_read_init failed");
		return;
	}
	user_fifo_read_proc(&fifo_cmd_args);
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

	pthread_t thread[1];
	pthread_attr_t a_thread_attr;
	pthread_attr_init(&a_thread_attr);
	pthread_attr_setstacksize(&a_thread_attr, PTHREAD_STACK_MIN*4);
	memset(&thread, 0, sizeof(thread));

	//get data from master
	if(pthread_create(&thread[0], &a_thread_attr, user_fifo_proc, NULL) != 0){
		err("user_fifo_proc failed!");
	} else {
		inf("user_fifo_proc success!");
	}
	if(thread[0] != 0){
		pthread_join(thread[0], NULL);
		inf("user_fifo_proc end!");
	}
    return 0;
}

