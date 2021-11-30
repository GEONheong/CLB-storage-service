#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "storageMyInfo.h"

int parseMsg_work(char* message){  //parsing message
    //temp pointer;
    char* msgTemp = NULL;
    //parsed item var		  
    char parse_buf[3][256] ={"NONE","NONE","NONE"};
    //command line buf
    char cmd[1024];
    //index
    int i=0;

    //parsing message
    msgTemp=strtok(message,"|");
    while (msgTemp != NULL){    
        strcpy(parse_buf[i],msgTemp);
        msgTemp = strtok(NULL,"|");
        i++;
    }

    //result parsed message work

    //if payload head is 'rmFile' working remove file
    if(strcmp(parse_buf[0],"rmFile") == 0){ 
        sprintf(cmd,"rm ./fileStore/%s/%s",parse_buf[1],parse_buf[2]);
        system(cmd);
        memset(&cmd,0x00,sizeof(cmd));
    }
    //if payload head is 'createDir' mkdir for new User
    else if(strcmp(parse_buf[0],"createDir") == 0)
    {
        sprintf(cmd,"mkdir ./fileStore/%s",parse_buf[1]);
        system(cmd);
        memset(&cmd,0x00,sizeof(cmd));
    }

	return 0;
}
