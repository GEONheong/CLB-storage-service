#include "currentStorageInfo.h"

extern char *getName(char *filepath);
extern void savePubkey(char* pubkey);

int inet_sock;
struct sockaddr_in server_inet;
char buf[BUFSIZ];

char filepath[256];
char filename[256];

storageInfo currentStorageInfo;

int main(int argc, char *argv[])
{
	if ((inet_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("error");
		exit(1);
	}

	memset((char*)&server_inet,'\0',sizeof(server_inet));
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
	
	while (1)
	{
		printf("%s",MENU);
		printf("Enter : ");
		gets(buf);

		if(sendto(inet_sock,buf,strlen(buf)+1,0,(struct sockaddr*)&server_inet,sizeof(server_inet)) == -1){
				perror("send fail");
				exit(1);
		}

		if(strcmp(buf,SENDFILE) == 0){ //select filesend
			
			//input send filepath
			printf("Input File Path: ");
			gets(filepath);
			strcpy(filename,getName(filepath));

			//send to CLB filename
			sendto(inet_sock,filename,strlen(filename)+1,0,
				(struct sockaddr*)&server_inet,sizeof(server_inet));

			if(strcmp(filename,"none") == 0){
				printf("Wrong Input...\n");
				printf("Input FILENAME or FILEPATH...\n");
				continue;
			}

			//recive from CLB storageInformation
			recvfrom(inet_sock,(storageInfo*)&currentStorageInfo,sizeof(currentStorageInfo),0,NULL,NULL);
			// printf("%s\n",currentStorageInfo.stor_filepath);
			// printf("%s\n",currentStorageInfo.stor_ip);
			// printf("%s\n",currentStorageInfo.stor_id);
			// printf("%s\n",currentStorageInfo.stor_pubkey);

			//modify "authorized_keys" file
			savePubkey(currentStorageInfo.stor_pubkey);

			//send file to storage(use scp) 
			sprintf(buf,"scp -p %s %s@%s:%s",filepath,currentStorageInfo.stor_id,currentStorageInfo.stor_ip,currentStorageInfo.stor_filepath);
			int ret = system(buf);
			printf("ret: %d\n",ret);
			
			//send to CLB "file send success" message
			if(ret == 0){
				sendto(inet_sock,SUCCESS,strlen(SUCCESS)+1,0,
				(struct sockaddr*)&server_inet,sizeof(server_inet));
			}
			else{
				sendto(inet_sock,FAIL,strlen(FAIL)+1,0,
				(struct sockaddr*)&server_inet,sizeof(server_inet));
			}
		}
		else if(strcmp(buf,GETFILE) == 0){ //select getfile

			//input filename
			printf("Input File Name: ");
			gets(filename);
			strcpy(filename,getName(filename));

			//send to CLB filename
			sendto(inet_sock,filename,strlen(filename)+1,0,
				(struct sockaddr*)&server_inet,sizeof(server_inet));

			if(strcmp(filename,"NONE") == 0){
				printf("Wrong Input...\n");
				printf("Input FILENAME or FILEPATH...\n");
				continue;
			}
			
			//recive from CLB storageInformation
			recvfrom(inet_sock,(storageInfo*)&currentStorageInfo,sizeof(currentStorageInfo),0,NULL,NULL); 
			
			//modify "authorized_keys" file
			savePubkey(currentStorageInfo.stor_pubkey);

			//send file to storage(use scp) 
			strcat(currentStorageInfo.stor_filepath,filename);
			sprintf(buf,"scp -p %s@%s:%s ./downloadFile",currentStorageInfo.stor_id,currentStorageInfo.stor_ip,currentStorageInfo.stor_filepath);
			int ret = system(buf);
			printf("ret: %d\n",ret);

			//send to CLB "file send success" message
			if(ret == 0){
				sendto(inet_sock,SUCCESS,strlen(SUCCESS)+1,0,
				(struct sockaddr*)&server_inet,sizeof(server_inet));
			}
			else{
				sendto(inet_sock,FAIL,strlen(FAIL)+1,0,
				(struct sockaddr*)&server_inet,sizeof(server_inet));
			}
		}
		else if(strcmp(buf,LOOKUPLIST) == 0){
			recvfrom(inet_sock,buf,sizeof(buf),0,NULL,NULL); // recv list
			printf("%s\n",buf);
		}
		else if(strcmp(buf,SERVICEEND) == 0){
			printf("Service terminate...\n");
			sleep(2);
			break;
		}
	}
	close(inet_sock); 
	return 0;
}
