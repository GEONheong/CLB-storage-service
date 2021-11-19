#include "../includeSet.h"
#include "storageInfo.h"

void getNewStorageInfo(struct sockaddr_in *client_addr, storageInfo *newStorage, char *buf)
{
    char *splitBuf = strtok(buf, "|");
    char *splitBuf_arr[4];
    int index = 0;

    splitBuf_arr[index] = splitBuf;
    while (splitBuf != NULL)
    {
        splitBuf = strtok(NULL, "|");
        index++;
        splitBuf_arr[index] = splitBuf;
    }

    strcpy(newStorage->stor_ip, inet_ntoa(client_addr->sin_addr));
    strcpy(newStorage->stor_pubkey, splitBuf_arr[0]);
    strcpy(newStorage->stor_filepath, splitBuf_arr[1]);
    strcpy(newStorage->stor_id, splitBuf_arr[2]);
    strcpy(newStorage->stor_ext, splitBuf_arr[3]);

    printf("-------------------------------\n");
    printf("IP :%s\nFilePath :%s\nID :%s\nEXT :%s\nPubKey :%s\n",
           newStorage->stor_ip, newStorage->stor_filepath,
           newStorage->stor_id, newStorage->stor_ext,
           newStorage->stor_pubkey);
    printf("-------------------------------\n");
}