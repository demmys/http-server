#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "socket.h"

typedef struct Server_tag{
    void (*delete)(struct Server_tag *);
    bool (*connect)(struct Server_tag *, int);
    bool (*listen)(struct Server_tag *, void (*)(int));
    struct addrinfo *_addrinfo;
    Socket *_sock;
    int _max;
} Server;

Server *newServer(const char *port);

#endif
