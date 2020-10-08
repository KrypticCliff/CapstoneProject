#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>


#define MAXBUFFSIZE 100
#define LOCALPORT   "8333"
#define BACKLOG     3

void sigchld_handler(int s)
{
    int saved_errno = errno;
    
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

int main(int argc, char* argv[])
{
    int status;
    int sfd;
    int client_sfd;
    int optval = 1;
    char buff[MAXBUFFSIZE];
    char ip[INET_ADDRSTRLEN];    
    socklen_t addr_size;

    struct addrinfo addr, *res, *a;
    struct sigaction sa;
    struct sockaddr_storage client_addr;

    // Set addr struct to 0
    memset(&addr, 0, sizeof(addr));
    addr.ai_family      = AF_INET;      // IP4
    addr.ai_socktype    = SOCK_STREAM;  // TCP Stream
    addr.ai_flags       = AI_PASSIVE;   // Use Current IP of Host
    addr.ai_protocol    = 0;

    // Sets up structure for HOST IP address
    if ((status = getaddrinfo(NULL, LOCALPORT, &addr, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

    for (a = res; a != NULL; a = a->ai_next)
    {
        // Initializes Socket File Descriptor *Create Err Check*
        if ((sfd = socket(a->ai_family, a->ai_socktype, a->ai_protocol)) == -1)
        {
            perror("server socket issue");
            continue;
        }

        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        // Returns 0 on success, -1 on err *Create Err Check*
        if (bind(sfd, res->ai_addr, res->ai_addrlen) == -1)
        {
            close(sfd);
            perror("Service Bind Error");
            continue;
        }

        break;
    }
    
    // Free up the res address
    freeaddrinfo(res);
    
    if (a == NULL)
    {
        fprintf(stderr, "Server failed to bind address");
        exit(1);
    }

    if (listen(sfd, BACKLOG) == -1)
    {
        perror("listen error");
        exit(1);
    }
   
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
 
    while(1)
    {
        // Blocking - Accepts incoming connection
        addr_size = sizeof(client_addr);
        client_sfd = accept(sfd, (struct sockaddr *) &client_addr, &addr_size);

        if (client_sfd == -1)
        {
            perror("accept error");
            continue;
        }

        //inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), ip, sizeof(ip));
        //printf("Server Connected to: %s\n", ip);
        
        if (!fork())
        {
            close(sfd);
            
            if (send(client_sfd, "Testing Text", 12, 0) == -1)
                perror("Send");
            
            close(client_sfd);
            exit(0);
        }
        close(client_sfd);    
    }

    return 0;
}
