#include "storageInfo.h"

extern char *getExt(char *filename);
extern char *compareExt_getStorKind_DB(char *ext);
extern int updateUserFileList_DB(char *user_id, char *filename, char *location);
extern char *getUserFileList_DB(char *user_id);


extern storageInfo *storageInfoArr;

extern int storageCount;

userInfo req_userInfo;

void *udp_server()
{
    pthread_detach(pthread_self());

    printf("[BEFORE storageInfoArr, addr, in udp_server] %x\n", storageInfoArr);

    char *ext = NULL;
    char recv_filename[256];

    int sock;
    char buf[BUFSIZ];
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

        //recv userInfo
        recvfrom(sock, (userInfo *)&req_userInfo, sizeof(req_userInfo), 0,
                 (struct sockaddr *)&client_in, &clientlen);

        printf("[USER %s] %s\n",req_userInfo.user_id,buf);

        if (strcmp(buf, _SENDFILE_MSG) == 0 || strcmp(buf, _GETFILE_MSG) == 0) //recv sendfile or getfile
        {
            //recv filename from client
            recvfrom(sock, buf, sizeof(buf), 0,
                     (struct sockaddr *)&client_in, &clientlen);
            strcpy(recv_filename, buf);

            //get file type
            ext = getExt(recv_filename);
            if (strcmp(ext, "NONE") == 0)
            {
                continue;
            }

            //send storaginformation to client(storage for that file type)
            char *location = compareExt_getStorKind_DB(ext);
            if (strcmp(location, "fail") == 0)
            {
                sendto(sock, (storageInfo *)&storageInfoArr[0], sizeof(storageInfoArr[0]), 0,
                               (struct sockaddr *)&client_in, sizeof(client_in));
                continue;
            }
            else
            {
                for (int i = 0; i < storageCount; i++)
                {
                    if (strcmp(storageInfoArr[i].stor_kind, location) == 0)
                    {
                        sendto(sock, (storageInfo *)&storageInfoArr[i], sizeof(storageInfoArr[i]), 0,
                               (struct sockaddr *)&client_in, sizeof(client_in));
                    }
                }
            }

            //recv from client success message
            recvfrom(sock, buf, sizeof(buf), 0,
                     (struct sockaddr *)&client_in, &clientlen);
            printf("Msg : %s\n", buf);

            //userFileList INSERT or UPDATE item in DB
            if (strcmp(buf, _FILE_SUCCESS) == 0)
            {
                int ret = updateUserFileList_DB(req_userInfo.user_id, recv_filename,location);

                if (ret)
                {
                    printf("%s\n", _LIST_UPDATE_SUCCESS);
                }
                else
                {
                    printf("%s\n", _LIST_UPDATE_FAIL);
                }
            }
        }
        else if (strcmp(buf, _LOOKUPLIST_MSG) == 0)
        {
            strcpy(buf,getUserFileList_DB(req_userInfo.user_id));

            sendto(sock, buf, strlen(buf)+1, 0,
                    (struct sockaddr *)&client_in, sizeof(client_in));
        }
        else
        {
            continue;
        }
    }

    close(sock);
    return 0;
}