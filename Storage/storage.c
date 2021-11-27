#include "storageMyInfo.h"

extern char* getPubkey(); //get ssh pubkey

int sock,sendInfoSock;
struct sockaddr_in server_inet;
char buf[BUFSIZ];
char* pubKey;

storageInfo storageMyInfo;

int main(int argc, char *argv[])
{
	strcpy(storageMyInfo.stor_ext,"txt");
	strcpy(storageMyInfo.stor_id,"pi");
	strcpy(storageMyInfo.stor_filepath,"/home/pi/Desktop/Geon/Storage/fileStore/");
	pubKey = getPubkey();
	strcpy(storageMyInfo.stor_pubkey,pubKey);

	if ((sendInfoSock = socket(AF_INET,SOCK_STREAM,0)) == -1){
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
	else
	{
		printf("Send Storage information\n");
		send(sendInfoSock, (storageInfo*)&storageMyInfo, sizeof(storageMyInfo), 0); // stroage info
		close(sendInfoSock);
	}

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("soket open error");
		exit(1);
	}

	close(sock);
	return 0;
}
