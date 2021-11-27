#include "currentStorageInfo.h"

extern char *getName(char *filepath);
extern void savePubkey(char *pubkey);

int inet_sock;
struct sockaddr_in server_inet;
char buf[BUFSIZ];

char filepath[256];
char filename[256];

storageInfo currentStorageInfo;

const int SENDFILE = 1;
const int GETFILE = 2;
const int LOOKUPLIST = 3;
const int SERVICEND = 4;

int main(int argc, char *argv[])
{
	int menuNum = 0;

	if ((inet_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("error");
		exit(1);
	}

	memset((char *)&server_inet, '\0', sizeof(server_inet));
	server_inet.sin_family = AF_INET;
	server_inet.sin_addr.s_addr = inet_addr(LB_IP);
	server_inet.sin_port = htons(9005);

	if (connect(inet_sock, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
	{
		perror("connect failed");
		exit(1);
	}
	else
	{
		printf("[Info] Inet socket : connected to the LB\n");
	}

	printf ("Welcom, %s\n",argv[1]);
	
	while (1)
	{
		printf("%s", MENU);
		printf("Enter : ");
		if(scanf("%d", &menuNum)){
			getchar();
		}else{
			printf("--------------------------------------\n");
			printf("Try again...(Wrong Input or No option)\n");
			printf("--------------------------------------\n");
			while(getchar() != '\n') //fflush
				continue;
			continue;
		}

		if (menuNum == SENDFILE) //select filesend
		{ 
			if (sendto(inet_sock, _SENDFILE_MSG, strlen(_SENDFILE_MSG)+1, 0, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
			{
				perror("send fail");
				exit(1);
			}

			//input send filepath
			printf("Input File Path: ");
			fgets(filepath,sizeof(filepath),stdin);
			filepath[strlen(filepath)-1]='\0';
			strcpy(filename, getName(filepath));

			//send to CLB filename
			sendto(inet_sock, filename, strlen(filename) + 1, 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));

			if (strcmp(filename, "NONE") == 0)
			{
				printf("--------------------------------------\n");
				printf("Wrong Input...\n");
				printf("Input FILENAME or FILEPATH...\n");
				printf("--------------------------------------\n");
				continue;
			}

			//recive from CLB storageInformation
			recvfrom(inet_sock, (storageInfo *)&currentStorageInfo, sizeof(currentStorageInfo), 0, NULL, NULL);
			// printf("%s\n",currentStorageInfo.stor_filepath);
			// printf("%s\n",currentStorageInfo.stor_ip);
			// printf("%s\n",currentStorageInfo.stor_id);
			// printf("%s\n",currentStorageInfo.stor_pubkey);

			//modify "authorized_keys" file
			savePubkey(currentStorageInfo.stor_pubkey);

			//send file to storage(use scp)
			printf("--------------------------------------\n");
			sprintf(buf, "scp -p %s %s@%s:%s", filepath, currentStorageInfo.stor_id, currentStorageInfo.stor_ip, currentStorageInfo.stor_filepath);
			int ret = system(buf);
			//printf("ret: %d\n", ret);
			printf("--------------------------------------\n");

			//send to CLB "file send success" message
			if (ret == 0)
			{
				sendto(inet_sock, _FILE_SUCCESS, strlen(_FILE_SUCCESS) + 1, 0,
					   (struct sockaddr *)&server_inet, sizeof(server_inet));
			}
			else
			{
				sendto(inet_sock, _FILE_FAIL, strlen(_FILE_FAIL) + 1, 0,
					   (struct sockaddr *)&server_inet, sizeof(server_inet));
			}
		}
		else if (menuNum == GETFILE) //select getfile
		{ 

			if (sendto(inet_sock, _GETFILE_MSG, strlen(_GETFILE_MSG) + 1, 0, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
			{
				perror("send fail");
				exit(1);
			}

			//input filename
			printf("Input File Name: ");
			fgets(filename,sizeof(filename),stdin);
			filename[strlen(filename)-1]='\0';
			strcpy(filename, getName(filename));

			//send to CLB filename
			sendto(inet_sock, filename, strlen(filename) + 1, 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));

			if (strcmp(filename, "NONE") == 0)
			{
				printf("--------------------------------------\n");
				printf("Wrong Input...\n");
				printf("Input FILENAME or FILEPATH...\n");
				printf("--------------------------------------\n");
				continue;
			}

			//recive from CLB storageInformation
			recvfrom(inet_sock, (storageInfo *)&currentStorageInfo, sizeof(currentStorageInfo), 0, NULL, NULL);

			//modify "authorized_keys" file
			savePubkey(currentStorageInfo.stor_pubkey);

			//get file to storage(use scp)
			printf("--------------------------------------\n");
			strcat(currentStorageInfo.stor_filepath, filename);
			sprintf(buf, "scp -p %s@%s:%s ./downloadFile", currentStorageInfo.stor_id, currentStorageInfo.stor_ip, currentStorageInfo.stor_filepath);
			int ret = system(buf);
			//printf("ret: %d\n", ret);
			printf("--------------------------------------\n");

			//send to CLB "file send success" message
			if (ret == 0)
			{
				sendto(inet_sock, _FILE_SUCCESS, strlen(_FILE_SUCCESS) + 1, 0,
					   (struct sockaddr *)&server_inet, sizeof(server_inet));
			}
			else
			{
				sendto(inet_sock, _FILE_FAIL, strlen(_FILE_FAIL) + 1, 0,
					   (struct sockaddr *)&server_inet, sizeof(server_inet));
			}
		}
		else if (menuNum == LOOKUPLIST)
		{
			if (sendto(inet_sock, _LOOKUPLIST_MSG, strlen(_LOOKUPLIST_MSG) + 1, 0, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
			{
				perror("send fail");
				exit(1);
			}
			
			recvfrom(inet_sock, buf, sizeof(buf), 0, NULL, NULL); // recv list
			printf("%s\n", buf);
		}
		else if (menuNum == SERVICEND)
		{
			printf("Service terminate...\n");
			sleep(2);
			break;
		}
	}
	close(inet_sock);
	return 0;
}
