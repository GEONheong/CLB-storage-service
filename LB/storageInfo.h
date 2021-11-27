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

#define _SENDFILE_MSG "_sendfile"
#define _GETFILE_MSG "_getfile"
#define _LOOKUPLIST_MSG "_lookuplist"
#define _SERVICEND_MSG "_servicend"
#define _SIGNIN_MSG "_signin"
#define _SIGNUP_MSG "_signup"

#define _SIGNIN_SUCCESS "Success sign in"
#define _SIGNIN_FAIL "Fail sign in"
#define _SIGNUP_SUCCESS "Success sign up"
#define _SIGNUP_FAIL "Fail sign up"

typedef struct storage {
    char stor_ip[15];
    char stor_filepath[256];
    char stor_id[8];
    char stor_ext[64];
    char stor_pubkey[1024];
}storageInfo;

typedef struct user{
    char user_id[50];
    char user_password[50];
}userInfo;


