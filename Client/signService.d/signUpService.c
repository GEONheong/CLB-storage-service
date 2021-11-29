#include "../clientMoniter.d/currentStorageInfo.h"


//called by "Geon_StorageService.sh"
//This is create new user,sign up service

int main(int argc, char *argv[]) // called like "./signUpService <user id> <user password>"
{
    //socket var
    int userInfoSock;
    struct sockaddr_in server_inet;
    char buf[BUFSIZ];

    //save user info;
    char *id = argv[1];
    char *password = argv[2];

    //new userInfo
    userInfo newUser;

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
    else  //if socket connected 
    {
        //send CLB(tcp_loginServer) sign up message
        send(userInfoSock, _SIGNUP_MSG, strlen(_SIGNUP_MSG) + 1, 0);
        
         //save user info in newUser;
        strcpy(newUser.user_id, id);
        strcpy(newUser.user_password, password);

        //send CLB(tcp_loginServer) sign up user info
        send(userInfoSock, (userInfo *)&newUser, sizeof(newUser), 0);
        printf("Send New User Information\n");

        //recv from CLB(tcp_loginServer) sign up success msg or fail msg
        recv(userInfoSock,buf, sizeof(buf), 0);

        printf("--------------------------------------\n");
        printf("%s\n",buf);
        printf("--------------------------------------\n");
        system("sh ./Geon_StorageService.sh"); //anyway call Geon_StorageService.sh(login menu program) 
    }

    close(userInfoSock);
    return 0;
}
