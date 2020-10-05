#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#define LOCALPORT   "8333"
#define BACKLOG     3

int main(int argc, char* argv[])
{
    int status;
    int sfd;
    int client_sfd;
    socklen_t addr_size;
    char ip[INET_ADDRSTRLEN];
    struct addrinfo addr, *res;

    struct sockaddr_storage client_addr;

    // Set addr struct to 0
    memset(&addr, 0, sizeof(addr));
    addr.ai_family      = AF_INET;      // IP4
    addr.ai_socktype    = SOCK_STREAM;  // TCP Stream
    addr.ai_flags       = AI_PASSIVE;   // Use Current IP of Host

    // Sets up structure for HOST IP address
    status = getaddrinfo(NULL, LOCALPORT, &addr, &res);
    
    if (status != 0)
    {
        fprintf(stderr, "An error has occurred: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    
    // Initializes Socket File Descriptor *Create Err Check*
    sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
    // Returns 0 on success, -1 on err *Create Err Check*
    bind(sfd, res->ai_addr, res->ai_addrlen);
    listen(sfd, BACKLOG);
    
    addr_size = sizeof(client_addr);
    client_sfd = accept(sfd, (struct sockaddr *) &client_sfd, &addr_size);

    freeaddrinfo(res);
    return 0;
}
