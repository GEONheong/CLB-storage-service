
#define LB_IP "192.168.0.20"

char *getPubkey();
int udpMulticast_recv();
int parseMsg_work(char* message);
int udpMulticast_recv();

typedef struct storage {
    char stor_ip[15];
    char stor_filepath[256];
    char stor_id[8];
    char stor_kind[64];
    char stor_pubkey[1024];
    char stor_pwd[20];
}storageInfo;

