#include <iostream>
#include <cstring>
#include <cstdio>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "./include/SocketBuild.h"

#define MAXBUFFSIZE 100
#define LOCALPORT   "8333"
#define BACKLOG     3

int main(int argc, char* argv[])
{
    int sfd, c_sfd;
    int status;
    int len;
    int optval = 1;

    struct addrinfo hint, *res;
    struct sockaddr_storage c_addr;

    socklen_t addrlen;
    fd_set readfd;

    // Sets up, configures, and create socket.
    sfd = CreateSocket(hint, res, AI_PASSIVE, NULL, LOCALPORT);

    // Listen on file descriptor. Current queues 3 incomming connections
    if (listen(sfd, BACKLOG) == -1)
    {
        perror("Error on Listen:");
        exit(EXIT_FAILURE);
    }

    // If connection attept succeeds, Establish connection to new file descriptor
    c_sfd = AcceptSocket(sfd, c_addr);

/*
    addrlen = sizeof(c_addr);
    if ((c_sfd = accept(sfd, (struct sockaddr*)&c_addr, &addrlen)) < 0)
    {
        close(sfd);
        perror("Error Accepting Connection:");
        exit(EXIT_FAILURE);    
    }
*/

    // Closes original file descriptor to prevent new connections.
    close(sfd);

    while(true)
    {
        char buf[MAXBUFFSIZE];
        int bytes_recv;

        FD_ZERO(&readfd);
        FD_SET(c_sfd, &readfd);
        FD_SET(STDIN_FILENO, &readfd);

        // Helps checks for incoming messages from Client
        select(FD_SETSIZE, &readfd, NULL, NULL, 0);

        bytes_recv = recv(c_sfd, buf, MAXBUFFSIZE, 0);

        if (bytes_recv < 0)
        {
            perror("Message Receive Failed");
            continue;
        }
        else if(bytes_recv == 0)
        {
            printf("Connection has been closed\n");
            break;
        }

        buf[bytes_recv] = 0;
        printf("Client: %s\n", buf);
    }
    close(c_sfd);
}
