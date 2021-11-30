#include "currentStorageInfo.h"

extern char *getName(char *filepath);
extern void savePubkey(char *pubkey);

int inet_sock;
struct sockaddr_in server_inet;
char buf[BUFSIZ];

char filepath[256];
char filename[256];

char dest_filepath[256];

storageInfo currentStorageInfo;
userInfo userMyInfo;

//menu number var
const int SENDFILE = 1;
const int GETFILE = 2;
const int LOOKUPLIST = 3;
const int REMOVEFILE = 4;
const int SERVICEND = 5;

int main(int argc, char *argv[])
{
	int menuNum = 0;

	// socket setting
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


	//welocom msg for user	
	system("clear");
	printf("#########################################\n");
	printf("Welcom, %s\n",argv[1]);
	printf("#########################################\n");

	//curret use user save user infomation
	strcpy(userMyInfo.user_id,argv[1]);
	strcpy(userMyInfo.user_password,"secert");

	while (1)
	{
		//show menu
		printf("%s", MENU);
		printf("Enter : ");
		if(scanf("%d", &menuNum)){
			getchar();
		}
		else{ //if input wrong thing
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
			//send sendfile msg
			if (sendto(inet_sock, _SENDFILE_MSG, strlen(_SENDFILE_MSG)+1, 0, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
			{
				perror("send fail");
				exit(1);
			}

			//input send filepath
			printf("Input File Path: ");
			fgets(filepath,sizeof(filepath),stdin);
			filepath[strlen(filepath)-1]='\0';
			strcpy(filename, getName(filepath)); //getName() return correct:'filename' error:'NONE' 

			//send my info
			sendto(inet_sock, (userInfo*)&userMyInfo, sizeof(userMyInfo), 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));

			//send to CLB filename
			sendto(inet_sock, filename, strlen(filename) + 1, 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));

			if (strcmp(filename, "NONE") == 0) //if filename is 'NONE' restart
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
			

			//if filename has 'no ext' or 'wrong ext'
			//CLB send currentStorageInfo.stor_kind=NOTTHING and restart
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
			strcpy(dest_filepath,currentStorageInfo.stor_filepath); //make user_id filepath
			sprintf(buf,"%s/",userMyInfo.user_id);
			strcat(dest_filepath,buf); // dest_filename = "currentStorageInfo.stor_ip + userMyInfo.user_id/" 
			//use scp if success return 0 
			//if fail return useByte
			sprintf(buf, "scp -p %s %s@%s:%s", filepath, currentStorageInfo.stor_id, currentStorageInfo.stor_ip, dest_filepath);
			int ret = system(buf);
			memset(&dest_filepath,'\0',sizeof(dest_filepath));
			printf("--------------------------------------\n");

			//send to CLB "file send success or fail" message
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
			//send getfile msg
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
			strcpy(dest_filepath,currentStorageInfo.stor_filepath);
			sprintf(buf,"%s/%s",userMyInfo.user_id,filename);
			strcat(dest_filepath,buf);
			sprintf(buf, "scp -p %s@%s:%s ./downloadFile", currentStorageInfo.stor_id, currentStorageInfo.stor_ip, dest_filepath);
			int ret = system(buf);
			memset(&dest_filepath,'\0',sizeof(dest_filepath));
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
		else if (menuNum == LOOKUPLIST) //select lookup filelist
		{
			//send lookuplist msg
			if (sendto(inet_sock, _LOOKUPLIST_MSG, strlen(_LOOKUPLIST_MSG) + 1, 0, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
			{
				perror("send fail");
				exit(1);
			}

			//send my info
			sendto(inet_sock, (userInfo*)&userMyInfo, sizeof(userMyInfo), 0,
				   (struct sockaddr *)&server_inet, sizeof(server_inet));
			
			//recv filelist
			recvfrom(inet_sock,buf,sizeof(buf),0,NULL,NULL);
			
			//show filelist
			system("clear");
			printf("################%s FileList#################\n",userMyInfo.user_id);
			printf("%s\n",buf);
			printf("Press ENTER quit:");
			getchar();
			system("clear");
		}
		else if(menuNum == REMOVEFILE){ //select remove file
			//send removefile msg
			if (sendto(inet_sock, _REMOVEFILE_MSG, strlen(_REMOVEFILE_MSG) + 1, 0, (struct sockaddr *)&server_inet, sizeof(server_inet)) == -1)
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
			
			system("clear");
		}
		else if (menuNum == SERVICEND) //select service end
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
