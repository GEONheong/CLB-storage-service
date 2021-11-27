#include "storageInfo.h"

#define TRUE 1
#define FALSE 0

extern int createNewUserInfo_DB(char* newUser_id,char* newUser_password);
extern int checkPreUserInfo_DB(char* preUser_id,char* preUser_password);

userInfo preUser,newUser;

void* tcp_loginServer()
{
    pthread_detach(pthread_self());
    
    //tcp socket var
    int access_sock, accept_sock;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len = sizeof(client_addr);
    char buf[BUFSIZ];

	if ((access_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket open error");
		exit(1);
	}

	memset((char *)&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(9998);

	if (bind(access_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind error");
		exit(1);
	}

	if (listen(access_sock, 15) == -1)
	{
		perror("listen error");
		exit(1);
	}
	
	while (TRUE)
	{

		if ((accept_sock = accept(access_sock, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
		{
			perror("accept error");
			exit(1);
		}
		else
		{
			recv(accept_sock, buf, sizeof(buf), 0);

            if(strcmp(buf,_SIGNIN_MSG) == 0){
                recv(accept_sock, (userInfo*)&preUser, sizeof(preUser), 0);

                int ret = checkPreUserInfo_DB(preUser.user_id,preUser.user_password);
                if(ret){
                    send(accept_sock,_SIGNIN_SUCCESS,strlen(_SIGNIN_SUCCESS)+1,0);
                }
                else{
                    send(accept_sock,_SIGNIN_FAIL,strlen(_SIGNIN_FAIL)+1,0);
                }
            }
            if(strcmp(buf,_SIGNUP_MSG) == 0){
                recv(accept_sock, (userInfo*)&newUser, sizeof(newUser), 0);
                int ret = createNewUserInfo_DB(newUser.user_id,newUser.user_password);

                if (ret){
                    send(accept_sock,_SIGNUP_SUCCESS,strlen(_SIGNUP_SUCCESS)+1,0);
                }else{
                    send(accept_sock,_SIGNUP_FAIL,strlen(_SIGNUP_FAIL)+1,0);
                }

            }
		}
	}

    close(accept_sock);
    close(accept_sock);
	return 0;
}
