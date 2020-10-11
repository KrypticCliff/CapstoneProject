#include <iostream>
// #include <cstring>
#include <cstdio>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
// #include <sys/wait.h>
#include <sys/select.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAXBUFFSIZE 100
#define LOCALPORT   "8333"
#define BACKLOG     3

/*
void sigchld_handler(int s)
{
    int saved_errno = errno;
    
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}
*/


int main(int argc, char* argv[])
{
    int sfd, c_sfd;
    int status;
    int len;
    //int bytes_recv;
    
    struct addrinfo hint, *res;
    struct sockaddr_in address;
    struct sockaddr_storage c_addr;

    socklen_t addrlen;
    fd_set readfd;

    memset(&hint, 0, sizeof(hint));
    hint.ai_flags    = AI_PASSIVE;
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    //hint.ai_protocol = LOCALPORT;

    // Used for DNS Lookup *Not Needed Yet*
    if ((status = getaddrinfo(NULL, LOCALPORT, &hint, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo error:%s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Sets up Socket Descriptor
    if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {   
        perror("Socket:");
        exit(EXIT_FAILURE);
    }

    if (bind(sfd, res->ai_addr, res->ai_addrlen) < 0)
    {
        perror("Socket Bind:");
        exit(EXIT_FAILURE);
    }

    // May need setsockopt() to prevent already used sockets
    
    if (listen(sfd, BACKLOG) < 0)
    {
        perror("Error on Listen:");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(c_addr);
    if ((c_sfd = accept(sfd, (struct sockaddr*)&c_addr, &addrlen)) < 0)
    {
        close(sfd);
        perror("Error Accepting Connection:");
        exit(EXIT_FAILURE);    
    }

    close(sfd);

    while(true)
    {
        char buf[MAXBUFFSIZE];
        int bytes_recv;

        FD_ZERO(&readfd);
        FD_SET(c_sfd, &readfd);
        FD_SET(STDIN_FILENO, &readfd);

        select(FD_SETSIZE, &readfd, NULL, NULL, 0);
        
        bytes_recv = recv(c_sfd, buf, MAXBUFFSIZE, 0);

        if (bytes_recv < 0)
        {
            perror("Message Receive Failed");
            continue;
        }
        else if(bytes_recv == 0)
        {
            printf("Connection has been closed");
            break;
        }

        buf[bytes_recv] = 0;
        printf("%s", buf);
    }
    close(c_sfd);
}

/*
    int status;
    int sfd;
    int client_sfd;
    int optval = 1;
    char buff[MAXBUFFSIZE];
    char ip[INET_ADDRSTRLEN];    
    bool socket_closed = false;
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

    // Initializes Socket File Descriptor *Create Err Check*
    if ((sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        perror("server socket issue");
        exit(-2);
    }

    // Returns 0 on success, -1 on err *Create Err Check*
    if (bind(sfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(sfd);
        perror("Service Bind Error");
        exit(-3);
    }
    
    // Free up the res address
    // freeaddrinfo(res);
    
    if (res == NULL)
    {
        fprintf(stderr, "Server failed to bind address");
        exit(1);
    }

    if (listen(sfd, BACKLOG) == -1)
    {
        perror("listen error");
        exit(1);
    }
   
    // Blocking - Accepts incoming connection
    addr_size = sizeof(client_addr);
    client_sfd = accept(sfd, (struct sockaddr *) &client_addr, &addr_size);

    if (client_sfd == -1)
    {
        perror("accept error");
        exit(1);
    }

    //inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), ip, sizeof(ip));
    //printf("Server Connected to: %s\n", ip);
    close(sfd);

    while(1)
    {
        char msg[12];
        fgets(msg, 12, stdin);
        
//        printf("The ALL MIGHTY Server says %s", msg);

        if (send(client_sfd, msg, 12, 0) == -1)
        {
            perror("Send");
            exit(0);
        }
    }
    close(client_sfd);
    return 0;
}

*/
