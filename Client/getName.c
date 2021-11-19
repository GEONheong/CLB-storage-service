#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* getName(char *filepath){
	static char buf[256]="";
	char* name = NULL;

	if(strstr(filepath,"/") != NULL){
		name = strrchr(filepath,'/');
		strcpy(buf,name+1);
	}
	else{
		strcpy(buf,filepath);
	}

	return buf;
}
