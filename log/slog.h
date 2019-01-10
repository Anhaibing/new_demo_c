#ifndef _SLOG_H_
#define _SLOG_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/time.h>

#define Hgray   "\033[1;30m"
#define Lgray   "\033[0;30m"
#define Hred    "\033[1;31m"
#define Lred    "\033[0;31m"
#define Hgreen  "\033[1;32m"
#define Lgreen  "\033[0;32m"
#define Hyellow "\033[1;33m"
#define Lyellow "\033[0;33m"
#define Hblue 	"\033[1;34m"
#define Lblue 	"\033[0;34m"
#define Hpurple "\033[1;35m"
#define Lpurple "\033[0;35m"
#define Hindigo "\033[1;36m"
#define Lindigo "\033[0;36m"
#define Hwhite 	"\033[1;37m"
#define Lwhite 	"\033[0;37m"
#define ColEnd 	"\033[0m"

typedef enum log_type_t{
    MIN_TYPE = -1,
    _RAW,
    _ERR,
    _WAR,
    _INF,
    _DBG,
    _TRC,
    MAX_TYPE
}log_type_t;

typedef struct log_ctr_t{
	char *name;
	char *color;
}log_ctr_t;

extern char log_ctrl_set[MAX_TYPE+1];

const char * get_last_name(const char *path);
void get_time_ms(char *ts,int size);
void log_init(char *ctrl, char *path);
void slog(
	log_type_t type,		//log level
	char lock_en,			//need mutex?
	char *log_ctrl_set,		//log E/D ctl
	const char *file,		//name of caller file
	const int line,			//line num of caller
	const char *fmt,		//for float print string
	...);
void raw_log(
	log_type_t type,
	char lock_en,
	char *log_ctrl_set,
	const char *fmt,
	...);

#define ANBIN_MIN(x,y) ((x)<(y)?(x):(y))
#define dlog(type, lock, x...)	slog(type, lock, log_ctrl_set, __FILE__, __LINE__, x);
#define rlog(type, lock, x...)	raw_log(type, lock, log_ctrl_set, x);

//这一组都是带宏的，适用于多线程程序调试，但不能用于信号回调函数中
#define raw(x...)	rlog(_RAW, 1, x);	//原始方式
#define err(x...)	dlog(_ERR, 1, x);
#define war(x...)	dlog(_WAR, 1, x);
#define inf(x...)	dlog(_INF, 1, x);
#define dbg(x...)	dlog(_DBG, 1, x);
#define trc(x...)	dlog(_TRC, 1, x);

//这一组宏 都是不带锁的，适用于信号回调函数中，单不能用于多线程程序调试
#define raw_nl(x...) 	rlog(_RAW,0,x);
#define err_nl(x...) 	dlog(_ERR,0,x);
#define war_nl(x...) 	dlog(_WAR,0,x);
#define inf_nl(x...) 	dlog(_INF,0,x);
#define dbg_nl(x...) 	dlog(_DBG,0,x);
#define trc_nl(x...) 	dlog(_TRC,0,x);

#define show_errno(ret, msg) ({\
	int num=(ret);\
	if(!num)num=errno;\
	err("%s failure,errno:%d[%s]",msg,num,strerror(num));\
})

//only print to console
#define clog(color,fmt,args...) ({ \
	FILE *fp = fopen("/dev/console", "a"); \
	if(!fp)fp=stdout;\
	fprintf(fp,"%s[%s %d]:\033[0m",color,get_last_name(__FILE__),__LINE__); \
	fprintf(fp,fmt,##args); \
	fprintf(fp,"\n"); \
	if(fp!=stdout)fclose(fp); \
})

//user can special a out dev, example /dev/pts/0
#define plog(path,fmt, args...) ({ \
	FILE *fp = fopen(path,"a"); \
	if (fp) { \
		fprintf(fp,"[%s %d]",get_last_name(__FILE__),__LINE__); \
		fprintf(fp,fmt,##args); \
		fprintf(fp,"\n"); \
		fclose(fp); \
	} \
})

#endif
