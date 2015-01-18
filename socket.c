#include "socket.h"

void socket_delete(Socket *this) {
    Socket *s;
    while(this) {
        s = this;
        free(this);
        this = s->next;
    }
}

Socket *newSocket() {
    Socket *s;

    s = (Socket *)malloc(sizeof(Socket));
    s->fd = -1;
    s->next = NULL;
    s->delete = socket_delete;

    return s;
}
