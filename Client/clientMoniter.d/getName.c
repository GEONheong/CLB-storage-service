#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* getName(char *filepath){  //parse filename in filepath
	static char filename[256]=""; //return filename
	char* nameTemp = NULL;		  //temp

	//have path (in other directory) ex) /home/geon/test.txt, ../geon/test.txt
	if(strstr(filepath,"/") != NULL)
	{
		nameTemp = strrchr(filepath,'/'); 
		strcpy(filename,nameTemp+1);
	}

	//don't have path(in same directory) ex) test.txt
	else
	{
		nameTemp = strrchr(filepath,'.');
		if(nameTemp == NULL){           //input wrong ex) test
			strcpy(filename,"NONE");
		}
		else{                          //input right ex) test.txt
			strcpy(filename,filepath);
		}	
	}

	return filename; //result filename;
}
