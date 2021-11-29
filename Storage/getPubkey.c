#include<stdio.h>
#include<string.h>


//get ssh public key in this machine(storage)
//from "/home/pi/.ssh/id_rsa.pub" and return public key to string
char* getPubkey(){
    FILE *file =NULL;
    static char buf[1024];

    if((file = fopen("/home/pi/.ssh/id_rsa.pub","rb")) == NULL){
        perror("file open error");
    }
    memset(buf,'\0',sizeof(buf));
    fread(buf,sizeof(char),sizeof(buf),file);
    //printf("%s",buf);
    fclose(file);
    return buf;
}