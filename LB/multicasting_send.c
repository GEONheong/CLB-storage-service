#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

//multicasting to all storage
//send message to all storage create newUser only directory
int createNewUserDir_send(char *newUser_id)
{
    //sock var
    int sendSock;

    //multicasting sock
    struct sockaddr_in mcast_group;
    struct ip_mreq mreq;

    //setting multicating sock
    memset(&mcast_group, 0x00, sizeof(mcast_group));
    mcast_group.sin_family = AF_INET;
    mcast_group.sin_port = htons(10005);
    inet_pton(AF_INET,"224.1.1.2",&mcast_group.sin_addr);

    mreq.imr_multiaddr = mcast_group.sin_addr;
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); // my ip

    if ((sendSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("sock open error");
        exit(1);
    }

    //send user id to storage
    sendto(sendSock, newUser_id, strlen(newUser_id) + 1, 0, (struct sockaddr *)&mcast_group, sizeof(mcast_group));

    printf("Send to storages : %s\n", newUser_id);
    close(sendSock);

    return 0;
}