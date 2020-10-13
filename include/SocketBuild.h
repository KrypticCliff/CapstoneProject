#ifndef SOCKETBUILD_H
#define SOCKETBUILD_H

#include <netdb.h>
#include <cstddef>

int CreateSocket(addrinfo &hint, addrinfo *&res, int flag, const char* node, const char* service);
int CreateSocket(addrinfo &hint, addrinfo *&res, int flag, const char* service);
int AcceptSocket(int sfd, sockaddr_storage c_addr);
void ConnectToServer(int sfd, addrinfo *&res);
size_t SendMessage(int sfd, const char* msg, size_t msize);

//int RecvMessage(int socket, char*& buf, size_t bsize);

#endif