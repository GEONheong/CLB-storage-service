#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pwd.h>
#include <unistd.h>
#include "storageMyInfo.h"

char *getPubkey(); //get ssh pubkey
int udpMulticast_recv();

//Send this storage machine infomation to CLB
//when storage is boot
int main()
{
	//socket var
	int sendInfoSock;
	struct sockaddr_in server_inet;
	char buf[BUFSIZ];
	char *pubKey;

	//this storage machine infomation
	storageInfo storageMyInfo;

	//get storage machine info var
	uid_t user_name;
	struct passwd *user_pw;
	char cwd[1024];

	//get storage machine info
	user_name = getuid();
	user_pw = getpwuid(user_name);
	getcwd(cwd, 1024);
	strcat(cwd, "/fileStore/");

	//this storage info
	strcpy(storageMyInfo.stor_kind, "video");
	strcpy(storageMyInfo.stor_id, user_pw->pw_name);
	strcpy(storageMyInfo.stor_filepath, cwd);
	pubKey = getPubkey();
	strcpy(storageMyInfo.stor_pubkey, pubKey);
	//strcpy(storageMyInfo.stor_pwd, "raspberry");

	//socket setting
	if ((sendInfoSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("sendInfoSock open error");
		exit(1);
	}

	memset((char *)&server_inet, '\0', sizeof(server_inet));
	server_inet.sin_family = AF_INET;
	server_inet.sin_addr.s_addr = inet_addr(LB_IP);
	server_inet.sin_port = htons(9999);

	if (connect(sendInfoSock, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
	{
		perror("socket connect error");
		exit(1);
	}
	else //if find CLB
	{
		//send this storage machine infomation
		printf("Send Storage information\n");
		send(sendInfoSock, (storageInfo *)&storageMyInfo, sizeof(storageMyInfo), 0); // stroage info
		close(sendInfoSock);
	}

	//start recever func
	udpMulticast_recv();
	return 0;
}
