#include <iostream>
#include <cstring>
#include <fstream>
#include <unistd.h>

#include "./include/SocketBuild.h"

#define MAXBUFFSIZE 50
#define LOCALPORT   "8333"
#define BACKLOG     3

int main(int argc, char* argv[])
{
    int sfd, c_sfd;
    int status;
    int len;
    int optval = 1;
    bool logger = true;

    struct addrinfo hint, *res;
    struct sockaddr_storage c_addr;
    struct timeval tv;

    socklen_t addrlen;
    fd_set readfd;

    // Opens file to append log
    std::ofstream log("ClientLog.txt", std::fstream::in | std::fstream::app);

    if (!log.is_open())
    {
        logger = false;
        printf("Client Logging Disabled. Unable to open file.");
    }

    if (log.fail())
        throw std::ios_base::failure(std::strerror(errno));

    // Timeout for Select() to pick up last printf()
    tv.tv_sec   = 1;        // 1 sec
    tv.tv_usec  = 500000;   // .5 sec

    // Sets up, configures, and create socket.
    sfd = CreateSocket(hint, res, AI_PASSIVE, LOCALPORT);

    // Listen on file descriptor. Current queues 3 incomming connections
    if (listen(sfd, BACKLOG) == -1)
    {
        perror("Error on Listen:");
        exit(EXIT_FAILURE);
    }

    // If connection attept succeeds, Establish connection to new file descriptor
    c_sfd = AcceptSocket(sfd, c_addr);

    // Closes original file descriptor to prevent new connections.
    close(sfd);

    while(true)
    {
        char buf[MAXBUFFSIZE];
        int bytes_recv;

        FD_ZERO(&readfd);
        FD_SET(c_sfd, &readfd);
        FD_SET(STDIN_FILENO, &readfd);
        FD_SET(STDOUT_FILENO, &readfd);

        // Helps checks for incoming messages from Client
        select(FD_SETSIZE, &readfd, NULL, NULL, &tv);

        bytes_recv = recv(c_sfd, buf, MAXBUFFSIZE, 0);

        if (bytes_recv < 0)
        {
            perror("Message Receive Failed");
            continue;
        }
        else if (bytes_recv == 0)
        {
            printf("Connection has been closed\n");
            break;
        }

        buf[bytes_recv] = 0;
        printf("Client: %s\n", buf);

        if (logger == true)
            log << "Client: " << buf << std::endl;
    }

    if (logger == true)
        log << "Connection has closed" << std::endl;
    
    log.close();
    close(c_sfd);
}
