#include "misc.h"

void argv_free(char *argv[]){
	int i=0;
	for(i=0;argv[i];i++){
		free(argv[i]);
	}
	free(argv);
	argv=NULL;
}

char** argl_to_argv(char argl[],int *pArgc){
	int i=0,count=0;
	if(!(argl && argl[0]))return NULL;
	char **argv=(char**)calloc(1,sizeof(char*));
	if(!argv)return NULL;
	argv[0]=argl;
	for(i=0;;i++){
		if(argl[i]==' '||argl[i]=='\0'||argl[i]=='\n'){
			count++;
			argv=(char**)realloc(argv,(count+1)*sizeof(char *));
			if(!argv)return NULL;
			argv[count]=argl+i+1;
			char *arg=calloc(1,argv[count]-argv[count-1]);
			if(!arg){
				argv_free(argv);
				return NULL;
			}
			strncpy(arg,argv[count-1],argv[count]-argv[count-1]-1);
			argv[count-1]=arg;
		}
		if(argl[i]=='\0'||argl[i]=='\n')break;
	}
	argv[count]=NULL;
	if(pArgc){
		*pArgc=count;
	}
	return argv;
}
