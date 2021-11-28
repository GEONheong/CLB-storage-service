#include "currentStorageInfo.h"

extern char *getName(char *filepath);
extern void savePubkey(char *pubkey);

int inet_sock;
struct sockaddr_in server_inet;
mcast_group.sin_addr;
char buf[BUFSIZ];

char filepath[256];
char filename[256];

storageInfo currentStorageInfo;
userInfo userMyInfo;

const int SENDFILE = 1;
const int GETFILE = 2;
const int LOOKUPLIST = 3;
const int REMOVEFILE = 4;
const int SERVICEND = 5;

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

	system("clear");
	
	printf("#########################################\n");
	printf("Welcom, %s\n",argv[1]);
	printf("#########################################\n");

	strcpy(userMyInfo.user_id,argv[1]);
	strcpy(userMyInfo.user_password,"secert");

	while (1)
	{
		printf("%s", MENU);
		printf("Enter : ");
		if(scanf("%d", &menuNum)){
			getchar();
		}else{
			system("clear");
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

			//send my info
			sendto(inet_sock, (userInfo*)&userMyInfo, sizeof(userMyInfo), 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));

			//send to CLB filename
			sendto(inet_sock, filename, strlen(filename) + 1, 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));

			if (strcmp(filename, "NONE") == 0)
			{
				system("clear");
				printf("--------------------------------------\n");
				printf("Wrong Input...\n");
				printf("Input FILENAME or FILEPATH...\n");
				printf("--------------------------------------\n");
				continue;
			}

			//recive from CLB storageInformation
			recvfrom(inet_sock, (storageInfo *)&currentStorageInfo, sizeof(currentStorageInfo), 0, NULL, NULL);

			if(strcmp(currentStorageInfo.stor_kind,NOTTHING) == 0){
				system("clear");
				printf("--------------------------------------\n");
				printf("you can't upload file(not support extension)\n");
				printf("--------------------------------------\n");
				continue;
			}

			//modify "authorized_keys" file
			savePubkey(currentStorageInfo.stor_pubkey);

			//send file to storage(use scp)
			system("clear");
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

			//send my info
			sendto(inet_sock, (userInfo*)&userMyInfo, sizeof(userMyInfo), 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));

			//send to CLB filename
			sendto(inet_sock, filename, strlen(filename) + 1, 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));

			if (strcmp(filename, "NONE") == 0)
			{
				system("clear");
				printf("--------------------------------------\n");
				printf("Wrong Input...\n");
				printf("Input FILENAME or FILEPATH...\n");
				printf("--------------------------------------\n");
				continue;
			}

			//recive from CLB storageInformation
			recvfrom(inet_sock, (storageInfo *)&currentStorageInfo, sizeof(currentStorageInfo), 0, NULL, NULL);

			if(strcmp(currentStorageInfo.stor_kind,NOTTHING) == 0){
				system("clear");
				printf("--------------------------------------\n");
				printf("you can't download file(not support extension)\n");
				printf("--------------------------------------\n");
				continue;
			}

			//modify "authorized_keys" file
			savePubkey(currentStorageInfo.stor_pubkey);

			//get file to storage(use scp)
			system("clear");
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

			//send my info
			sendto(inet_sock, (userInfo*)&userMyInfo, sizeof(userMyInfo), 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));
			
			recvfrom(inet_sock,buf,sizeof(buf),0,NULL,NULL);

			system("clear");
			printf("################%s FileList#################\n",userMyInfo.user_id);
			printf("%s\n",buf);
			printf("Press ENTER quit:");
			getchar();
			system("clear");
		}
		else if(menuNum == REMOVEFILE){
			if (sendto(inet_sock, _REMOVEFILE_MSG, strlen(_REMOVEFILE_MSG) + 1, 0, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
			{
				perror("send fail");
				exit(1);
			}

			//send my info
			sendto(inet_sock, (userInfo*)&userMyInfo, sizeof(userMyInfo), 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));
		
		}
		else if (menuNum == SERVICEND)
		{
			printf("--------------------------------------\n");
			printf("Service terminate...\n");
			printf("--------------------------------------\n");
			sleep(2);
			break;
		}
	}
	close(inet_sock);
	return 0;
}
