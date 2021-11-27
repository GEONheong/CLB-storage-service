#include "../clientMoniter.d/currentStorageInfo.h"

int main(int argc, char *argv[])
{
    int userInfoSock;
    struct sockaddr_in server_inet;
    char buf[BUFSIZ];

    char *id = argv[1];
    char *password = argv[2];

    userInfo preUser;

    char bufForSys[BUFSIZ];

    if ((userInfoSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("sendInfoSock open error");
        exit(1);
    }

    memset((char *)&server_inet, '\0', sizeof(server_inet));
    server_inet.sin_family = AF_INET;
    server_inet.sin_addr.s_addr = inet_addr(LB_IP);
    server_inet.sin_port = htons(9998);

    if (connect(userInfoSock, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
    {
        perror("socket connect error");
        exit(1);
    }
    else
    {
        send(userInfoSock, _SIGNIN_MSG, strlen(_SIGNIN_MSG) + 1, 0);

        strcpy(preUser.user_id, id);
        strcpy(preUser.user_password, password);

        send(userInfoSock, (userInfo *)&preUser, sizeof(preUser), 0);

        printf("Send Present User Information\n");

        recv(userInfoSock,buf, sizeof(buf), 0);
        
        if(strcmp(buf,_SIGNIN_SUCCESS) == 0){
            printf("%s\n",_SIGNIN_SUCCESS);
            sprintf(bufForSys,"./clientMoniter.d/clientMoniter %s",preUser.user_id);
            system(bufForSys);
        }
        else if(strcmp(buf,_SIGNIN_FAIL) == 0){
            printf("%s\n",_SIGNIN_FAIL);
            system("sh ./myStorageService.sh");
        }
    }

    

    close(userInfoSock);
    return 0;
}
