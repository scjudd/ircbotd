#ifndef TCP_H
#define TCP_H

/*
 * Open a TCP connection to a given host and port
 * Returns file descriptor on success, -1 on error
 */

int tcp_open(const char *host, const char *port);

#endif
