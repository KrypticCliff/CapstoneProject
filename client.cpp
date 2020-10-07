#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define LOCALPORT       "8333"

int main(int argc, char* argv[])
{
    int sfd;
    int status;

    struct addrinfo hints, *res;

    hints.ai_flags      = 0;
    hints.ai_protocol   = 0;
    hints.ai_family     = AF_INET;
    hints.ai_socktype   = SOCK_STREAM;

    // Check if IP was included. Close is not
    if (argc != 2)
    {
        printf("You forgot the address\n");
        return -1;
    }

    status = getaddrinfo(argv[1], LOCALPORT, &hints, &res);

    if (status != 0)
    {
        fprintf(stderr, "An error has occurred: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    bind(sfd, res->ai_addr, res->ai_addrlen);

    std::cout << "TEST***\n";
    
    if (connect(sfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(sfd);
        //printf("Connecting...\n");
    }

    freeaddrinfo(res);
}
