#include<stdio.h>
#include<string.h>

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

// int main(){
//     char* buf = getPubkey();

//     printf("%s",buf);
//     return 0;
// }