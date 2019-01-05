#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include "slog.h"

static FILE *fp = NULL;
char log_ctrl_set[MAX_TYPE+1] = "";
pthread_mutex_t line_lock = PTHREAD_MUTEX_INITIALIZER;	//互斥锁，用于线程间同步

//print level color
log_ctr_t logCtrArray[MAX_TYPE] = {
	[_ERR] = {"ERR"		,Hred		},
	[_WAR] = {"WAR"		,Hyellow	},
	[_INF] = {"INF"		,Hgreen		},
	[_DBG] = {"DBG"		,Hblue		},
	[_TRC] = {"TRC"		,Hpurple	},
};

void get_time_ms(char *ts,int size){
	
	time_t time_now;
	time(&time_now);
	snprintf(ts,size,"%s", ctime(&time_now));
	//struct timeval tv={0};
	//if(gettimeofday(&tv,NULL)<0){
	//	return;
	//}
	//snprintf(ts,size,"%013lu",tv.tv_sec*1000+tv.tv_usec/1000);
}

const char * get_last_name(const char*path) {
	int i=0;
	const char* last_lever=path;
	for(i=0;path[i];i++){
		if(path[i]=='/'){
			last_lever=path+i+1;
		}
	}
	return last_lever;
}

void log_init(char *ctrl, char *path) {
    if (ctrl){
		strncpy(log_ctrl_set, ctrl, ANBIN_MIN(sizeof(log_ctrl_set), strlen(ctrl)));
	}
    if (path){
		pthread_mutex_lock(&line_lock);
		if(!(fp = fopen(path, "a"))){
			printf("fopen %s failed, errno = %d\n", path, errno);
			fp = stdout;
		}
		pthread_mutex_unlock(&line_lock);
	}
}

inline void slog(log_type_t type, char lock_en, char * log_ctrl_set, const char * file, const int line, const char * fmt, ...){
	if('1' == log_ctrl_set[type]){
		if(lock_en)
			pthread_mutex_lock(&line_lock);
		if(!fp)
			fp = stdout;
		va_list args;
		char ts[26] = "";
		get_time_ms(ts, sizeof(ts));
		fprintf(fp, "[%s%s%s\033[0m %s:%d]:", ts, logCtrArray[type].color, logCtrArray[type].name, get_last_name(file), line);
		va_start(args, fmt);
		vfprintf(fp, fmt, args);
		va_end(args);
		fprintf(fp, "\n");
		if(lock_en)
			pthread_mutex_unlock(&line_lock);
	}
}

inline void raw_log(log_type_t type, char lock_en, char * log_ctrl_set, const char * fmt, ...){
	if('1' == log_ctrl_set[type]){
		if(lock_en)
			pthread_mutex_lock(&line_lock);
		if(!fp)
			fp = stdout;
		va_list args;
		va_start(args, fmt);
		vfprintf(fp, fmt, args);
		va_end(args);
		if(lock_en)
			pthread_mutex_unlock(&line_lock);
	}
}