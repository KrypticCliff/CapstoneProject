#include <iostream>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <unistd.h>

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

    hint.ai_flags    = 0;
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    if (argc != 3)
    {
        printf("Address and Port Needed. <IP> <Port>");
        return -1;
    }

    memset(&hint, 0, sizeof(hint));
    if ((status = getaddrinfo(argv[1], argv[2], &hint, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo Error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    //printf("Connecting to Server...");
    if (connect(sfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(sfd);
        perror("Connect Failed:");
    }

    printf("You are now connected to ... You may now chat!\n");

    while(1)
    {
        char msg[20];
        fgets(msg, 20, stdin);

        if (send(sfd, msg, 20, 0) == -1)
        {
            perror("Send Fail:");
            continue;
        }
    }
    close(sfd);
}
/*
{
    int sfd;
    int status;
    int len; //, bytes_sent;
    struct addrinfo hints, *res;
    fd_set readfd;

    // Check if IP was included. Close is not
    if (argc != 2)
    {
        printf("You forgot the address\n");
        return -1;
    }

    hints.ai_flags      = 0;
    hints.ai_protocol   = 0;
    hints.ai_family     = AF_INET;
    hints.ai_socktype   = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], LOCALPORT, &hints, &res)) != 0)
    {
        fprintf(stderr, "An error has occurred: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (connect(sfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(sfd);
        perror("Client Connect");
    }
    
//    freeaddrinfo(res);
    
    while(1)
    {
        char buffer[MAXDATASIZE];
        int bytes_sent = 0;
        bool msg_sent = false;
        
        FD_ZERO(&readfd);
        FD_SET(sfd, &readfd);
        FD_SET(STDIN_FILENO, &readfd);

        if (select(FD_SETSIZE, &readfd, NULL, NULL, NULL) < 0)
        {
            perror("select:");
            exit(-1);
        }
        
        if ((bytes_sent = recv(sfd, buffer, MAXDATASIZE, 0)) == -1)
        {
            perror("Receive");
            exit(1);
        }
        else
            msg_sent = true;
    
        buffer[bytes_sent] = 0;
        
        if (msg_sent == true)
        {
            printf("%s", buffer);
            //printf("Received: %s\n", buffer);
            msg_sent = false;
            int bytes_sent = 0;
        }
    }
}
*/
