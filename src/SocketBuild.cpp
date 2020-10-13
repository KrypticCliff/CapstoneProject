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

    //struct addrinfo hint, *res;

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

size_t SendMessage(int sfd, const char*& msg, size_t msize)
{
    size_t retval;

    if ((retval = send(sfd, msg, msize, 0)) == -1)
        perror("Send Fail:");
        //continue;

    return retval;
}

/*
int RecvMessage(int socket, char& buf, size_t bsize)
{
        int bytes_recv = recv(socket, buf, MAXBUFFSIZE, 0);

        if (bytes_recv < 0)
            perror("Message Receive Failed");

        else if(bytes_recv == 0)
        {
            printf("Connection has been closed\n");
            break
        }

        return bytes_recv;
}
*/