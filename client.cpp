#include <iostream>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <unistd.h>

#include "./include/SocketBuild.h"

#define LOCALPORT       "8333"
#define MAXDATASIZE     100

int main(int argc, char* argv[])
{
    int sfd, s_sfd;
    int status;
    int len;

    struct addrinfo hint, *res;
    
    socklen_t addrlen;
    fd_set readfd;

    if (argc != 3)
    {
        printf("Address and Port Needed. <IP> <Port>\n");
        return -1;
    }

    // Create socket struct, create socket, and output socket 
    sfd = CreateSocket(hint, res, 0, argv[1], argv[2]);

    // Connect to Server
    ConnectToServer(sfd, res);

    printf("You are now connected to ... You may now chat!\n");

    while(1)
    {
        int retval;
        char msg[50];
        fgets(msg, 50, stdin);  // Data to STDIN

        // Send currently blocking. 
        // Takes up to 50 char per message sent
        retval = SendMessage(sfd, msg, 50);
    }
    close(sfd);
}
