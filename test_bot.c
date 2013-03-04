#include "bot.h"
#include <ev.h>

int main(int argc, char *argv[])
{
    irc_bot *b = bot_init();
    struct ev_loop *loop = ev_default_loop(0);

    bot_host(b, "localhost");
    bot_port(b, "7000");
    bot_nick(b, "b0t");
    bot_join(b, "#adullam");

    if (bot_connect(loop, b) == -1)
    {
        bot_free(b);
        return -1;
    }

    while (1)
        ev_loop(loop, 0);

    bot_free(b);
    return 0;
}
