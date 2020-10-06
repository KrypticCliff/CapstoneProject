#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>

#define LOCALPORT       "8333"

int main(int argc, char* argv[])
{
    int sfd;
    int status;

    struct addrinfo hints, *res;

    hints.ai_flags      = AI_PASSIVE;
    hints.ai_family     = AF_INET;
    hints.ai_socktype   = SOCK_STREAM;

    status = getaddrinfo(NULL, LOCALPORT, hints, res);

    if (status != 0)
    {
        fprintf(stderr, "An error has occurred: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    bind(sfd, res->ai_addr, res->ai_addrlen)

    //connect();
}
