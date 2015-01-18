#include <stdio.h>
#include "server.h"

void receive(int fd) {
    int n;
    char buf[1024];

    while((n = read(fd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);
    }
}

int main(int argc, char const* argv[]) {
    Server *s = newServer(argc > 1 ? argv[1] : "80");
    if(!s) {
        fprintf(stderr, "%s: Faild to prepare server.\n", argv[0]);
        return EXIT_FAILURE;
    }
    if(!s->connect(s, 5)) {
        fprintf(stderr, "%s: Faild to connect.\n", argv[0]);
        return EXIT_FAILURE;
    }
    if(!s->listen(s, receive)) {
        fprintf(stderr, "%s: Faild to listen.\n", argv[0]);
        return EXIT_FAILURE;
    }
    s->delete(s);
    return EXIT_SUCCESS;
}
