#include "storageMyInfo.h"

extern char *getPubkey(); //get ssh pubkey
extern int createNewUserDir_recv();

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
	
	//this storage info
	strcpy(storageMyInfo.stor_kind, "docs");
	strcpy(storageMyInfo.stor_id, "pi");
	strcpy(storageMyInfo.stor_filepath, "/home/pi/Desktop/Geon/Storage/fileStore/");
	pubKey = getPubkey();
	strcpy(storageMyInfo.stor_pubkey, pubKey);

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
	createNewUserDir_recv();
	return 0;
}
