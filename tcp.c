#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int tcp_open(const char *host, const char *port)
{
    struct addrinfo hints = { 0, PF_UNSPEC, SOCK_STREAM, 0, 0, NULL, NULL, NULL };
    struct addrinfo *res0, *res;
    int sockfd;

    int gai_errno, tmp_errno;
    char *err_cause;

    gai_errno = getaddrinfo(host, port, &hints, &res0);

    // Couldn't resolve host to IP address
    if (gai_errno)
    {
        fprintf(stderr, "Couldn't resolve %s to IP address: %s\n",
                host, gai_strerror(gai_errno));
        return -1;
    }

    sockfd = -1;
    for (res = res0; res != NULL; res = res->ai_next)
    {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        // Couldn't create socket
        if (sockfd < 0)
        {
            sockfd = -1;
            err_cause = "socket";
            continue;
        }

        // Couldn't connect
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0)
        {
            tmp_errno = errno;
            close(sockfd);
            errno = tmp_errno;

            sockfd = -1;
            err_cause = "connect";
            continue;
        }

        break;
    }

    freeaddrinfo(res0);

    if (sockfd == -1)
    {
        fprintf(stderr, "Couldn't connect to %s:%s: %s\n", host, port, err_cause);
        return -1;
    }

    if (fcntl(sockfd, F_SETFD, 1) == -1)
    {
        fprintf(stderr, "Couldn't set close-on-exec flag for sockfd\n");
        return -1;
    }

    return sockfd;
}
