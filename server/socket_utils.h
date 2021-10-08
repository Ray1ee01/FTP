// socket processes for server

#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>

// socket(), bind() and listen(): build a socket,bind and listen to a port
int ListenBind(int port);

// build a connection
int AcceptConnection(int listen_fd);


#endif