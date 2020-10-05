#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    int status;
    int sfd;
    char ip[INET_ADDRSTRLEN];
    struct addrinfo addr, *res;

    // Set addr struct to 0
    memset(&addr, 0, sizeof(addr));
    addr.ai_family      = AF_INET;      // IP4
    addr.ai_socktype    = SOCK_STREAM;  // TCP Stream
    addr.ai_flags       = AI_PASSIVE;   // Use Current IP of Host

    // Sets up structure for HOST IP address
    status = getaddrinfo(NULL, "8333", &addr, &res);
    
    if (status != 0)
    {
        fprintf(stderr, "An error has occurred: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    bind(sfd, res->ai_addr, res->ai_addrlen);

    // Creates Socket File Descriptor from Addr setup
    //int sfd = socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);

    // Checks if there are more IP addresses connected to this host (DNS Lookup)    


/*
    for (s = res; s != NULL; s = s->ai_next)
    {
        int sfd = socket(s->ai_family, s->ai_socktype, s->ai_protocol);
        struct sockaddr_in *saddr = (struct sockaddr_in *) s->ai_addr;
        int port = ntohs(saddr->sin_port);

        inet_ntop(res->ai_family, &saddr->sin_addr, ip, INET_ADDRSTRLEN);

        if (sfd < 0)
        {
            fprintf(stderr, "An Error Has Occurred with the File Descriptor");
        }

        printf("Current Setup:\nIP:%s\nPort:%d\n", ip, port);
    }
*/
    //while (true)    
    
    freeaddrinfo(res);
    return 0;
}
