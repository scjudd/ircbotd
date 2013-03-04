#include "bot.h"
#include "tcp.h"
#include "queue.h"
#include <ev.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

irc_bot* bot_init()
{
    irc_bot *b;

    if ( (b = malloc(sizeof(*b))) == NULL ||
         (b->irc_host = calloc(MAX_HOST_LEN+1, sizeof(char))) == NULL ||
         (b->irc_port = calloc(MAX_PORT_LEN+1, sizeof(char))) == NULL ||
         (b->irc_nick = calloc(MAX_NICK_LEN+1, sizeof(char))) == NULL ||
         (b->irc_io = malloc(sizeof(ev_io))) == NULL )
    { bot_free(b); return NULL; }

    b->sockfd = -1;
    b->irc_in_queue = queue_init();
    b->irc_io->data = b;

    return b;
}

int bot_free(irc_bot *b)
{
    if (b != NULL)
    {
        if (b->irc_host != NULL)
            free(b->irc_host);
        if (b->irc_port != NULL)
            free(b->irc_port);
        if (b->irc_nick != NULL)
            free(b->irc_nick);
        if (b->irc_io != NULL)
            free(b->irc_io);
        if (b->irc_in_queue != NULL)
            queue_free(b->irc_in_queue);
        free(b);
    }
    return 0;
}

int bot_host(irc_bot *b, char *host)
{
    if (strlen(host) > MAX_HOST_LEN)
        return -1;
    strcpy(b->irc_host, host);
    return 0;
}

int bot_port(irc_bot *b, char *port)
{
    if (strlen(port) > MAX_PORT_LEN)
        return -1;
    strcpy(b->irc_port, port);
    return 0;
}

int bot_nick(irc_bot *b, char *nick)
{
    if (strlen(nick) > MAX_NICK_LEN)
        return -1;
    strcpy(b->irc_nick, nick);
    // TODO: implementation
    return 0;
}

int bot_join(irc_bot *b, char *channel)
{
    if (strlen(channel) > MAX_CHAN_LEN)
        return -1;
    // TODO: implementation
    return 0;
}

static void ev_irc_cb(EV_P_ ev_io *w, int revents)
{
    irc_bot *b = w->data;
    size_t msg_space = 512;
    size_t offset = 0, msg_len = 0, buff_len = 0;
    char *msg, *msg_start, *msg_end, *buff_end;
    char buff[RECV_BUFF_SIZE];

    if (revents & EV_READ)
    {
        msg = malloc(sizeof(char) * msg_space);
        buff_end = buff + RECV_BUFF_SIZE;

        while ((buff_len = recv(b->sockfd, &buff, RECV_BUFF_SIZE, 0)) > 0)
        {
            // find first msg
            msg_start = buff;
            msg_end = strstr(msg_start, "\r\n");

            while (msg_end != NULL)
            {
                // calculate msg length
                msg_len = (msg_end - msg_start + offset) / sizeof(char);

                // if there's not enough space allocated for this msg, realloc
                if (msg_space < msg_len + 1)
                {
                    while (msg_space < msg_len + 1)
                        msg_space = msg_space << 1;
                    msg = realloc(msg, sizeof(char) * msg_space);
                }

                // copy the msg, add nul-byte
                strncpy(msg + offset, msg_start, msg_len);
                msg[msg_len] = '\0';

                // reset offset
                if (offset > 0) offset = 0;

                // do something with message
                printf("msg: %s\n", msg);
                queue_append(b->irc_in_queue, msg);

                // find the next message
                msg_start = msg_end + 2;
                msg_end = strstr(msg_start, "\r\n");

                // if we have a partial message
                if (msg_end == NULL && (buff + buff_len * sizeof(char) + 1) == buff_end)
                {
                    msg_len = (buff_end - msg_start) / sizeof(char);
                    strncpy(msg, msg_start, msg_len);
                    offset = msg_len - 1;
                }
            }
        }
        free(msg);
    }
}

int bot_connect(EV_P_ irc_bot *b)
{
    if (strlen(b->irc_host) == 0 || strlen(b->irc_port) == 0)
        return -1;

    // connect
    if ((b->sockfd = tcp_open(b->irc_host, b->irc_port)) == -1)
        return -1;

    // make socket non-blocking
    fcntl(b->sockfd, F_SETFD, (fcntl(b->sockfd, F_GETFD) | O_NONBLOCK));

    ev_io_init(b->irc_io, ev_irc_cb, b->sockfd, EV_READ);
    ev_io_start(loop, b->irc_io);

    return 0;
}
