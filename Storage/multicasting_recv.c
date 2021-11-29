#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

//recver muticasting_send(in LB dir)
//recv new user id
//create new user private directory
int createNewUserDir_recv(){
    //socket var
    int read_sock;
    int flag;
    struct sockaddr_in from;
    int addrlen=sizeof(from);

    //multicasting socket var
    struct sockaddr_in mcast_group;
    struct ip_mreq mreq;

    //new user id var
    char newUser_id[256];

    //use in system() commad buf
    char cmd[256];

    //socket setting
    memset(&mcast_group,0x00,sizeof(mcast_group));
    mcast_group.sin_family = AF_INET;
    mcast_group.sin_port = htons(10005);
    inet_pton(AF_INET,"224.1.1.2",&mcast_group.sin_addr);

    if((read_sock=socket(AF_INET,SOCK_DGRAM,0)) < 0){
        perror("socket open error");
        exit(1);
    }

    mreq.imr_multiaddr = mcast_group.sin_addr;
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); // my ip

    //join in multicasting channel
    if(setsockopt(read_sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)
    {
        perror("add group error");
        exit(1);
    }

    flag=1;
    if(setsockopt(read_sock,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag)) < 0)
    {
        perror("socket resueaddr error");
        exit(1);
    }

    if(bind(read_sock,(struct sockaddr*)&mcast_group,sizeof(mcast_group)) < 0)
    {
        perror("bind error");
        exit(1);
    }
    
    while(1){
        
        memset(newUser_id,0x00,256);
        memset(cmd,0x00,256);

        //recv user id
        recvfrom(read_sock,newUser_id,256,0,(struct sockaddr*)&from,&addrlen);
        printf("[NEW USER]%s\n",newUser_id);

        //mkdir user id directory
        sprintf(cmd,"mkdir ./fileStore/%s",newUser_id);
        system(cmd);       
    }

    close(read_sock);
    return 0;
}