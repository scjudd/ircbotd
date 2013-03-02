#include "bot.h"
#include <stdlib.h>
#include <string.h>

irc_bot* bot_init()
{
    irc_bot *b;

    if ( (b = malloc(sizeof(*b))) == NULL ||
         (b->irc_host = calloc(MAX_HOST_LEN+1, sizeof(char))) == NULL ||
         (b->irc_port = calloc(MAX_PORT_LEN+1, sizeof(char))) == NULL ||
         (b->irc_nick = calloc(MAX_NICK_LEN+1, sizeof(char))) == NULL )
    { bot_free(b); return NULL; }

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

int bot_connect(irc_bot *b) {
    if (b->irc_host == NULL || b->irc_port == NULL)
        return -1;
    // TODO: implementation
    return 0;
}
