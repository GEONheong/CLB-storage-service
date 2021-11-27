#include "storageInfo.h"

extern char *getExt(char *filename);

extern storageInfo *storageInfoArr;
extern int storageCount;

// char* docsExt[6] = {"doc","docx","html","hwp","pdf","txt"};
// char* videoExt[5] = {"avi","mkv","mov","flv","mp4","wmv"};
// char* audioExt[5] = {"mp3","wav","wma","raw","ogg"};

void *udp_server()
{

    pthread_detach(pthread_self());

    printf("[BEFORE storageInfoArr, addr, in udp_server] %x\n", storageInfoArr);

    char *ext = NULL;

    int sock;
    char buf[256];
    struct sockaddr_in server_in, client_in;
    int clientlen = sizeof(client_in);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("udp sock open error");
        exit(1);
    }

    memset((char *)&server_in, '\0', sizeof(server_in));
    server_in.sin_family = AF_INET;
    server_in.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_in.sin_port = htons(9005);

    if (bind(sock, (struct sockaddr *)&server_in, sizeof(server_in)))
    {
        perror("udp bind error");
        exit(1);
    }

    while (1)
    {
        //recv menu
        printf("waiting msg....\n");
        recvfrom(sock, buf, sizeof(buf), 0,
                 (struct sockaddr *)&client_in, &clientlen);
        
        printf("%s\n",buf);

        if (strcmp(buf, _SENDFILE_MSG) == 0 || strcmp(buf, _GETFILE_MSG) == 0) //recv sendfile or getfile
        {
            //recv filename from client
            recvfrom(sock, buf, sizeof(buf), 0,
                     (struct sockaddr *)&client_in, &clientlen);

            //get file type
            ext = getExt(buf);
            if(strcmp(ext,"NONE") == 0){
                continue;
            }

            //send storaginformation to client(storage for that file type)
            for (int i = 0; i < storageCount; i++)
            {
                if (strcmp(storageInfoArr[i].stor_ext, ext) == 0)
                {
                    sendto(sock, (storageInfo *)&storageInfoArr[i], sizeof(storageInfoArr[i]), 0,
                           (struct sockaddr *)&client_in, sizeof(client_in));
                }
            }

            //recv from client success message
            recvfrom(sock, buf, sizeof(buf), 0,
                     (struct sockaddr *)&client_in, &clientlen);
            printf("Msg : %s\n",buf);
        }
        else if (strcmp(buf, _LOOKUPLIST_MSG) == 0)
        {
        }
        else{
            continue;
        }
    }

    close(sock);
    return 0;
}