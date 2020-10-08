#include <iostream>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define LOCALPORT       "8333"
#define MAXDATASIZE     100

int main(int argc, char* argv[])
{
    int sfd;
    int status;

    char buffer[MAXDATASIZE];
    int len, bytes_sent;

    struct addrinfo hints, *res;

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
    
    freeaddrinfo(res);

    if ((bytes_sent = recv(sfd, buffer, MAXDATASIZE-1, 0)) == -1)
    {
        perror("Receive");
        exit(1);
    }
    
    buffer[bytes_sent] = '\0';
    printf("Received: %s\n", buffer);
}
