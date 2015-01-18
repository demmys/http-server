#include "server.h"

void server_delete(Server *this) {
    this->_sock->delete(this->_sock);
    freeaddrinfo(this->_addrinfo);
    free(this);
}

bool server_connect(Server *this, int queue_size) {
    int sock;
    struct addrinfo *i;
    Socket **s;

    s = &(this->_sock);
    for(i = this->_addrinfo; i; i = i->ai_next) {
        sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
        if(sock < 0) {
            continue;
        }
        if(sock >= FD_SETSIZE) {
            close(sock);
            continue;
        }
        if(bind(sock, i->ai_addr, i->ai_addrlen) < 0) {
            close(sock);
            continue;
        }
        if(listen(sock, queue_size) < 0) {
            close(sock);
            continue;
        }
        if(sock > this->_max) {
            this->_max = sock;
        }
        *s = newSocket();
        (*s)->fd = sock;
        s = &((*s)->next);
    }
    if(!this->_sock) {
        return false;
    }
    return true;
}

bool server_listen(Server *this, void (*callback)(int)) {
    int l;
    fd_set fds, fds_sel;
    struct sockaddr_storage from;
    socklen_t fromlen;
    Socket *s;
    pid_t pid;

    FD_ZERO(&fds);
    for(s = this->_sock; s; s = s->next) {
        FD_SET(s->fd, &fds);
    }
    while(true) {
        fds_sel = fds;
        if(select(this->_max + 1, &fds_sel, NULL, NULL, NULL) < 0) {
            return false;
        }
        for(s = this->_sock; s; s = s->next) {
            if(FD_ISSET(s->fd, &fds_sel)) {
                fromlen = sizeof(from);
                l = accept(s->fd, (struct sockaddr *)&from, &fromlen);
                if(l < 0) {
                    continue;
                }
                pid = fork();
                if(pid == -1) {
                    close(l);
                    return false;
                }
                if(pid == 0) {
                    close(s->fd);
                    callback(l);
                    exit(EXIT_SUCCESS);
                }
                close(l);
            }
        }
    }
}

Server *newServer(const char *port) {
    int err;
    struct addrinfo h;

    Server *s = (Server *)malloc(sizeof(Server));
    s->delete = server_delete;
    s->connect = server_connect;
    s->listen = server_listen;
    s->_sock = NULL;
    s->_max = -1;

    memset(&h, 0, sizeof(struct addrinfo));
    h.ai_family = AF_UNSPEC;
    h.ai_socktype = SOCK_STREAM;
    h.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, port, &h, &(s->_addrinfo))) {
        return NULL;
    }
    return s;
}
