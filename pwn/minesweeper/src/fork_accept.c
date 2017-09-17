// A sample forking/listening CTF binary (shamelessly reverse engineered
// from an old Ghost in the Shellcode challenge). You don't have to use
// this exact code, but make sure that your forking/listening servers
// are not susceptible to the problems mentioned in the comments.
#include <arpa/inet.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include "malloc.h"

const uint16_t port = 31337;

extern int ms(int fd);

// It is recommended to have wrappers like this around send and recv.
// Remember that send/recv can return without reading all n requested
// bytes.
ssize_t recvlen(int fd, char *buf, size_t n) {
    ssize_t rc;
    size_t nread = 0;
    char *nl = 0;
    while (nread < n) {
        rc = recv(fd, buf + nread, n - nread, 0);
        if (rc == -1) {
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (rc == 0) {
            if (nread == 0) {
                return -1;
            }
            break;
        }
        nread += rc;
        nl = strstr(buf, "\n");
        if (nl) {
            *nl = 0;
            break;
        }
    }
    return nread;
}

ssize_t sendlen(int fd, const char *buf, size_t n) {
    ssize_t rc;
    size_t nsent = 0;
    while (nsent < n) {
        rc = send(fd, buf + nsent, n - nsent, 0);
        if (rc == -1) {
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            }
            return -1;
        }
        nsent += rc;
    }
    return nsent;
}

ssize_t sendstr(int fd, const char *str) {
    return sendlen(fd, str, strlena(str));
}



// The connection handling function.
// Put your vulnerable code here :-)
int handle(int fd) {
    return ms(fd);
}

int main(int argc, char **argv) {
    int rc;
    int opt;
    int sockfd;
    int clientfd;
    pid_t pid;
    struct sockaddr_in saddr = {0};
    fprintf(stderr, "Server started");
    // Setting the SIGCHLD handler to SIG_IGN prevents child
    // processes from becoming zombies (so you do not need to
    // call wait() on them).
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        fputs("Failed to set SIGCHLD handler.", stderr);
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Set SO_REUSEADDR. Otherwise, if the server crashes for
    // any reason, you will have to wait for sockets to time
    // out before you can reuse the port.
    opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt,
                   sizeof(opt)) != 0) {
        perror("setsockopt");
        return 1;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &saddr,
             sizeof(saddr)) != 0) {
        perror("bind");
        return 1;
    }

    if (listen(sockfd, 20) != 0) {
        perror("listen");
        return 1;
    }

    while (1) {
        clientfd = accept(sockfd, NULL, NULL);
        if (clientfd == -1) {
            perror("accept");
            continue;
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            close(clientfd);
            continue;
        }
        if (pid == 0) {
            // Avoid tons of long-running processes sticking around.
            alarm(1000000);

            // If you do not close the socket fd, someone who
            // exploits the service could call accept() on it and
            // hijack other people's connections.
            close(sockfd);

            // The server is started as root and drops privileges
            // after forking and before handling the request.
            // Otherwise, someone who exploits the service can
            // kill, ptrace, or otherwise interfere with the server.
            fprintf(stderr, "New user connected");
            rc = handle(clientfd);

            close(clientfd);
            _exit(rc);
        }

        // If you forget to close the client fd,  you could run
        // out of file descriptors (it also makes the connection fd
        // hard to predict, which can be annoying for someone
        // writing an exploit - if you want to do this on purpose,
        // use dup2 with a random fd instead :-P).
        close(clientfd);
    }

    return 0;
}
