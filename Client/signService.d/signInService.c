#include "../clientMoniter.d/currentStorageInfo.h"

//called by "Geon_StorageService.sh"
//This is login,sign in service

int main(int argc, char *argv[])  // called like "./signInService <user id> <user password>"
{
    //socket var
    int userInfoSock;
    struct sockaddr_in server_inet;
    char buf[BUFSIZ];

    //save user info;
    char *id = argv[1];
    char *password = argv[2];

    //present userInfo
    userInfo preUser;

    //use in system()
    char bufForSys[BUFSIZ];

    //socket setting
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
    else  // if socket connected 
    {
        //send CLB(tcp_loginServer) sign in message
        send(userInfoSock, _SIGNIN_MSG, strlen(_SIGNIN_MSG) + 1, 0);

        //save user info in preUser;
        strcpy(preUser.user_id, id);
        strcpy(preUser.user_password, password);

        //send CLB(tcp_loginServer) sign in user info
        send(userInfoSock, (userInfo *)&preUser, sizeof(preUser), 0);
        printf("Send Present User Information\n");

        //recv from CLB(tcp_loginServer) sign in success msg or fail msg
        recv(userInfoSock,buf, sizeof(buf), 0);
        
        printf("--------------------------------------\n");
        if(strcmp(buf,_SIGNIN_SUCCESS) == 0){   //if success
            printf("%s\n",_SIGNIN_SUCCESS);
            printf("--------------------------------------\n");
            sprintf(bufForSys,"./clientMoniter.d/clientMoniter %s",preUser.user_id); //call clientMoninter(main menu program)
            system(bufForSys);
        }
        else if(strcmp(buf,_SIGNIN_FAIL) == 0){ //if fail 
            printf("%s\n",_SIGNIN_FAIL);
            printf("--------------------------------------\n");
            system("sh ./Geon_StorageService.sh"); //call Geon_StorageService.sh(login menu program) 
        }   
    }
    close(userInfoSock);
    return 0;
}
