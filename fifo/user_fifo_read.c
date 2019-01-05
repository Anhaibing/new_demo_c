#include "user_fifo.h"

int user_fifo_read_init(fifo_cmd_t *pArgs){
	inf("%s", __func__);
	switch(pArgs->type){
	case FIFO_CMD_UNSPECIFIED_FIFO:
	case FIFO_CMD_SPECIFIED_FIFO:{
		pArgs->ops_ptr = unix_fifo_ops_create(pArgs->path, 0);
		if(!pArgs->ops_ptr)
			return -1;
		return 0;
	}
	case FIFO_CMD_UNSPECIFIED_PIPE:
	case FIFO_CMD_SPECIFIED_PIPE:{
		unixFifoOps_t *ptr = calloc(1, sizeof(unixFifoOps_t));
		if(!ptr){
			err("calloc failed");
			return -2;
		}
		ptr->fd = pArgs->fd;
		pArgs->ops_ptr = ptr;
		return 0;
	}
	default:
		return -2;	
	}
}

int user_fifo_read_proc(fifo_cmd_t *pArgs){
	inf("%s", __func__);
	char buf[1024] = "";
	for(;;){
		bzero(buf, sizeof(buf));
		int get_len = unix_fifo_ops_read(pArgs->ops_ptr, buf, sizeof(buf));
		inf("ops_str: %s", pArgs->ops_ptr);
		inf("read buf %s", buf);
		inf("get len %d", get_len);
		if(get_len > 0){
			char *idx = buf-FMT_SUBFIX_LEN;
			char *cmd = NULL;
			for(;;){
				cmd=idx=memmem(idx+FMT_SUBFIX_LEN, buf+get_len-idx-FMT_SUBFIX_LEN, FMT_PREFIX,FMT_PREFIX_LEN);
				inf("cmd %s", cmd);
				if(!cmd)break;
				idx= memmem(cmd+FMT_PREFIX_LEN, buf+get_len-idx-FMT_PREFIX_LEN, FMT_SUBFIX, FMT_SUBFIX_LEN);
				if(!idx)break;
				if(pArgs->handle){
					pArgs->handle(cmd+FMT_PREFIX_LEN,idx-cmd-FMT_PREFIX_LEN);
					continue;
				}
				user_fifo_read_cmd_handle(pArgs,cmd+FMT_PREFIX_LEN,idx-cmd-FMT_PREFIX_LEN);
			}
		}
	}	
}

int user_fifo_read_cmd_handle(fifo_cmd_t* pArgs, char *buf, size_t len){
	inf("%s", __func__);
	int ret = 0, argc = 0, cmpLen = 0;
	item_arg_t item_arg = {0};
	dbg("argl:%s",buf);
	char **argv = argl_to_argv(buf, &argc);
	if(!argv)
		return -1;
	size_t i = 0;
	for(; pArgs->tbl[i].cmd; i++){
		cmpLen = ANBIN_MIN(strlen(argv[0]), strlen(pArgs->tbl[i].cmd));
		if(strncmp(pArgs->tbl[i].cmd, argv[0],cmpLen))
			continue;
		item_arg.argc = argc;
		item_arg.argv = argv;
		item_arg.args = pArgs->tbl[i].args;
		ret = pArgs->tbl[i].handle(&item_arg);
		ret = ret<0?-2:0;
		goto exit;
	}
	ret = -3;
exit:
	if(0 == ret){
		dbg("%s excute succeed!", argv[0]);
	} else if(-2 == ret){
		err("%s excute failure!",argv[0]);
	} else if(-3 == ret){
		err("%s not found!",argv[0]);
	}
	FREE(argv);
	return ret;
}

