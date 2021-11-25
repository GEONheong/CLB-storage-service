#include "storageInfo.h"

#define TRUE 1
#define FALSE 0

extern void* udp_server();

//tcp socket var
int access_sock, accept_sock;
struct sockaddr_in server_addr, client_addr;
int client_addr_len = sizeof(client_addr);
char buf[BUFSIZ];


int storageCount = 1;

storageInfo *storageInfoArr = NULL;
pthread_t udpServerThr;

int main(int argc, char *argv[])
{
	storageInfoArr = (storageInfo *)malloc(sizeof(storageInfo) * storageCount);

	if ((access_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket open error");
		exit(1);
	}

	memset((char *)&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

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

	pthread_create(&udpServerThr,NULL,udp_server,NULL);
	printf("[BEFORE storageInfoArr, addr] %x\n",storageInfoArr);

	while (TRUE)
	{

		if ((accept_sock = accept(access_sock, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
		{
			perror("accept error");
			exit(1);
		}
		else
		{
			recv(accept_sock, (storageInfo*)&storageInfoArr[storageCount-1], sizeof(storageInfoArr[storageCount-1]), 0);
			
			strcpy(storageInfoArr[storageCount-1].stor_ip, inet_ntoa(client_addr.sin_addr));
			printf("%s\n",storageInfoArr[storageCount-1].stor_filepath);
			printf("%s\n",storageInfoArr[storageCount-1].stor_ip);
			printf("%s\n",storageInfoArr[storageCount-1].stor_id);
			printf("%s\n",storageInfoArr[storageCount-1].stor_pubkey);
			printf("%s\n",storageInfoArr[storageCount-1].stor_ext);

			storageCount ++;
			storageInfoArr = (storageInfo*)realloc(storageInfoArr,sizeof(storageInfo)*storageCount);
			printf("[AFTER realloc, addr] %x\n", storageInfoArr);
		}
	}

	return 0;
}
