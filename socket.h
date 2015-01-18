#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <stdlib.h>

typedef struct Socket_tag{
    int fd;
    struct Socket_tag *next;
    void (*delete)(struct Socket_tag *);
} Socket;

Socket *newSocket();

#endif
