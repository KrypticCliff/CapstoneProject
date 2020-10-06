#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAXBUFFSIZE 100
#define LOCALPORT   "8333"
#define BACKLOG     3

int main(int argc, char* argv[])
{
    int status;
    int sfd;
    int client_sfd;
    socklen_t addr_size;
    char buff[MAXBUFFSIZE];
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
    
    // addr_size = sizeof(client_addr);
    // Blocking - Accepts incoming connection
    client_sfd = accept(sfd, (struct sockaddr *) &client_sfd, (socklen_t *) sizeof(client_addr));

    if (client_sfd != 0)
    {
        fprintf(stderr, "An error has occurred: Socket Accept Failed\n");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        int datasize = recv(client_sfd, buff, MAXBUFFSIZE-1, 0);

        for (int i = 0; i < datasize-1; i++)
            printf("%c", buff[i]);
    }

    freeaddrinfo(res);
    return 0;
}
