#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    int status;
    char ip[INET_ADDRSTRLEN];
    struct addrinfo addr;
    struct addrinfo *s, *res;

    if (argc != 2)
    {
        fprintf(stderr, "Include port number");
        return 1;
    }
    
    // Set addr struct to 0
    memset(&addr, 0, sizeof(addr));

    addr.ai_family      = AF_INET;      // IP4
    addr.ai_socktype    = SOCK_STREAM;  // TCP Stream
    addr.ai_flags       = AI_PASSIVE;   // Use Current IP of Host

    // Sets up structure for HOST IP address
    status = getaddrinfo(NULL, argv[1], &addr, &res);
    //status = getaddrinfo(argv[1], NULL, &addr, &res);

    if (status != 0)
    {
        fprintf(stderr, "An error has occurred: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    
    for (s = res; s != NULL; s = s->ai_next)
    {
        int fd = socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
        printf("%d", s->ai_protocol);
    }
    

    //printf("%s", ip);

    return 0;
}
