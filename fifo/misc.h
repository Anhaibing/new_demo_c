#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>

#define ADD_HANDLE_ITEM(name,args) {#name,name##_handle,args},
#define FREE(x) if(x){free(x);x=NULL;}

void argv_free(char *argv[]);
char** argl_to_argv(char argl[],int *pArgc);
