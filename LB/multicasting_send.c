#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

//multicasting to all storage
//1.send message to all storage create newUser only directory
//2.send message to all storage remove file
int udpMulticast_send(char *message)
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
    sendto(sendSock, message, strlen(message) + 1, 0, (struct sockaddr *)&mcast_group, sizeof(mcast_group));

    printf("Send to storages : %s\n", message);
    close(sendSock);

    return 0;
}