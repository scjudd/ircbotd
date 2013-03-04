#ifndef BOT_H
#define BOT_H

#include <ev.h>
#include "queue.h"

#define MAX_HOST_LEN 64     // HOST_NAME_MAX from /usr/include/bits/local_lim.h
#define MAX_PORT_LEN 5      // 0 - 65535
#define MAX_NICK_LEN 30     // http://www.irc.org/tech_docs/005.html
#define MAX_CHAN_LEN 50     // http://www.irc.org/tech_docs/005.html

#define RECV_BUFF_SIZE 2048

typedef struct irc_bot {
    char    *irc_host, *irc_port;
    char    *irc_nick;
    int     sockfd;
    ev_io   *irc_io;
    queue   *irc_in_queue;
} irc_bot;

irc_bot* bot_init();                // initialize a new bot
int bot_free(irc_bot*);             // free dynamically allocated memory
int bot_host(irc_bot*, char*);      // set irc server host
int bot_port(irc_bot*, char*);      // set irc server port
int bot_nick(irc_bot*, char*);      // set irc nickname
int bot_join(irc_bot*, char*);      // join channel on irc server
int bot_connect(EV_P_ irc_bot*);    // connect to irc server

#endif
