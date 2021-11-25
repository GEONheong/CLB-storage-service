#include<stdio.h>
#include<stdlib.h>
#include<string.h>


char* getExt(char* filename){
	static char buf[256]="";
	char* ext = NULL;

	ext = strrchr(filename,'.');		
	if(ext == NULL)
		return "NONE";

	strcpy(buf,ext+1);
	
	return buf;
}
