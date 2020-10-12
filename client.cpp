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
    
    // Set hint with 0 bytes
    memset(&hint, 0, sizeof(hint));

    // Setup addrinfo struct with user-inputted server info
    if ((status = getaddrinfo(argv[1], argv[2], &hint, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo Error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Establish Socket File Descriptor
    sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // Connect to Server
    if (connect(sfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(sfd);
        perror("Connect Failed:");
        exit(EXIT_FAILURE);
    }

    printf("You are now connected to ... You may now chat!\n");

    while(1)
    {
        int retval;
        char msg[50];
        fgets(msg, 50, stdin);  // Data to STDIN

        /*
        retval = send(sfd, msg, 50, 0);
        
        if (retval == -1)
        {
            perror("Send Fail:");
            continue;
        }
        else if (retval == 0)
        {
            perror("Send Disconnect:");
            exit(EXIT_SUCCESS);
        }
        */

        // Send currently blocking. 
        // Takes up to 50 char per message sent
        if (send(sfd, msg, 50, 0) == -1)
        {
            perror("Send Fail:");
            continue;
        }
    }
    close(sfd);
}
