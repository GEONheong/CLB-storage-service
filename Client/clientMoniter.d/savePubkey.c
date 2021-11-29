#include<stdio.h>
#include<string.h>

void savePubkey(char* pubkey){  //if called save "pubkey" string in "/home/pi/.ssh/authorized_keys" files
    FILE *file =NULL;
    char buf[1024];
    memset(buf,0,sizeof(buf));
    strcpy(buf,pubkey);

    if((file = fopen("/home/pi/.ssh/authorized_keys","wb")) == NULL){
        perror("file open error");
    }

    fwrite(buf,sizeof(char),strlen(buf),file);

    fclose(file);
}