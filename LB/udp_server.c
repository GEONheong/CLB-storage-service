#include "storageInfo.h"

extern char *getExt(char *filename);

extern storageInfo *storageInfoArr;
extern int size;

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
        recvfrom(sock, buf, sizeof(buf), 0,
                 (struct sockaddr *)&client_in, &clientlen);

        if (strcmp(buf, SENDFILE) == 0 || strcmp(buf, GETFILE) == 0) //recv sendfile or getfile
        {
            //recv filename from client
            recvfrom(sock, buf, sizeof(buf), 0,
                     (struct sockaddr *)&client_in, &clientlen);
 
            //get file type
            ext = getExt(buf);

            //send storaginformation to client(storage for that file type)
            for (int i = 0; i < size; i++)
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
            printf("Success Msg : %s\n",buf);
        }
        // else if(strcmp(buf,GETFILE) == 0)
        // {

        // }
        else if (strcmp(buf, LOOKUPLIST) == 0)
        {
        }
    }

    close(sock);
    return 0;
}