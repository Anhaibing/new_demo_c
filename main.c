#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "user_fifo.h"

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
	unixFifoOps_t *fifoPtr = user_fifo_write_init("./anbinFIFO");
	if(!fifoPtr) {
		err("user_fifo_write_init failed");
		return -1;
	}
	user_fifo_write_fmt(fifoPtr,"anbintest 5");	//for test
	inf("this is inf");
	war("this is war");
	err("this is err");
	trc("this is inf");
	dbg("this is dbg");

    return 0;
}
