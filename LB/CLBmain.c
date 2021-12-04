#include "main.h"

#define TRUE 1
#define FALSE 0

//storage info count
int storageCount = 2;

// //will save in here storage info
storageInfo *storageInfoArr = NULL;

void* udp_server();
void* tcp_loginServer();

int main(int argc, char *argv[])
{
	//tcp socket var
	int access_sock, accept_sock;
	struct sockaddr_in server_addr, client_addr;
	int client_addr_len = sizeof(client_addr);
	char buf[BUFSIZ];

	//thread var
	pthread_t udpServerThr,tcploginThr;

	//allocation memory in storageInfoArray
	storageInfoArr = (storageInfo *)malloc(sizeof(storageInfo) * storageCount);

	//first storage info have notting (empty sotrage info)
	strcpy(storageInfoArr[0].stor_ip,NOTTHING);
    strcpy(storageInfoArr[0].stor_filepath,NOTTHING);
    strcpy(storageInfoArr[0].stor_id,NOTTHING);
    strcpy(storageInfoArr[0].stor_kind,NOTTHING);
    strcpy(storageInfoArr[0].stor_pubkey,NOTTHING);
	//strcpy(storageInfoArr[0].stor_pwd,NOTTHING);

	//socket setting
	if ((access_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket open error");
		exit(1);
	}

	memset((char *)&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(LB_IP);
	server_addr.sin_port = htons(9999);

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

	pthread_create(&tcploginThr,NULL,tcp_loginServer,NULL); // thread login server
	pthread_create(&udpServerThr,NULL,udp_server,NULL); //thread main program server
	printf("[BEFORE storageInfoArr, addr] %x\n",storageInfoArr);

	while (TRUE)
	{
		if ((accept_sock = accept(access_sock, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
		{
			perror("accept error");
			exit(1);
		}
		else  //if accept socket (storage approach)
		{
			//send that storage infomation
			recv(accept_sock, (storageInfo*)&storageInfoArr[storageCount-1], sizeof(storageInfoArr[storageCount-1]), 0); 
			
			//print storage info
			strcpy(storageInfoArr[storageCount-1].stor_ip, inet_ntoa(client_addr.sin_addr));
			// printf("%s\n",storageInfoArr[storageCount-1].stor_filepath);
			// printf("%s\n",storageInfoArr[storageCount-1].stor_ip);
			// printf("%s\n",storageInfoArr[storageCount-1].stor_id);
			// printf("%s\n",storageInfoArr[storageCount-1].stor_pubkey);
			// printf("%s\n",storageInfoArr[storageCount-1].stor_kind);
			// printf("%s\n",storageInfoArr[storageCount-1].stor_pwd);

			//sizeup sotrageinfo Array
			storageCount ++;
			storageInfoArr = (storageInfo*)realloc(storageInfoArr,sizeof(storageInfo)*storageCount);
			printf("[AFTER realloc, addr] %x\n", storageInfoArr);
		}
	}

	return 0;
}
