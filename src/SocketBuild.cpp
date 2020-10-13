#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
//#include <errno.h>

#include "../include/SocketBuild.h"

int CreateSocket(addrinfo &hint, addrinfo *&res, int flag, const char* node, const char* service)
{
    int sfd;
    int status;
    int optval = 1;

    memset(&hint, 0, sizeof(hint));
    hint.ai_flags       = flag;
    hint.ai_family      = AF_INET;
    hint.ai_socktype    = SOCK_STREAM;

    if ((status = getaddrinfo(node, service, &hint, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo error:%s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Sets up Socket File Descriptor
    if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {   
        perror("Socket:");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
    {
        perror("setsockopt:");
        exit(EXIT_FAILURE);
    }

    if (flag == AI_PASSIVE)
    {
        if (bind(sfd, res->ai_addr, res->ai_addrlen) == -1)
        {
            close(sfd);
            perror("Socket Bind:");
            exit(EXIT_FAILURE);
        }
    }

    return sfd;
}

void ConnectToServer(int sfd, addrinfo *&res)
{
    if (connect(sfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(sfd);
        perror("Connect Failed:");
        exit(EXIT_FAILURE);
    }
}

int AcceptSocket(int sfd, sockaddr_storage c_addr)
{
    int c_sfd;
    socklen_t addrlen = sizeof(c_sfd);

    if ((c_sfd = accept(sfd, (struct sockaddr*) &c_addr, &addrlen)) < 0)
    {
        close(sfd);
        perror("Error Accepting Connection:");
        exit(EXIT_FAILURE);
    }
    return c_sfd;
}

size_t SendMessage(int sfd, const char* msg, size_t msize)
{
    size_t retval;

    retval = send(sfd, msg, msize, 0);

    if (retval == -1)
        perror("Send Fail:");
    else if (retval == 0)
        perror("Socket is closed");

    return retval;
}

/* *TESTING* Cant use within loop due to recv() blocking state
int RecvMessage(int socket, char& buf, size_t bsize)
{
    int count = 1;
    int bytes_recv = recv(socket, buf, MAXBUFFSIZE, 0);

    if (bytes_recv < 0)
        perror("Message Receive Failed");

    else if(bytes_recv == 0)
    {
        printf("Connection has been closed\n");
        exit(EXIT_SUCCESS);
    }
    
    return bytes_recv;
}
*/