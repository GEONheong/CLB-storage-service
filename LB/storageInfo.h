#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>

#define LB_IP "127.0.0.1"

#define SENDFILE "1"
#define GETFILE "2"
#define LOOKUPLIST "3"
#define SERVICEEND "4"

typedef struct storage {
    char stor_ip[15];
    char stor_filepath[256];
    char stor_id[8];
    char stor_ext[64];
    char stor_pubkey[1024];
}storageInfo;

typedef struct clinet{
    
}clientInfo;
//static storageInfo *storageInfoArr = NULL;


