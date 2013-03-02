#include "bot.h"

int main(int argc, char *argv[])
{
    irc_bot *b = bot_init();

    bot_host(b, "localhost");
    bot_port(b, "7000");
    bot_nick(b, "b0t");
    bot_join(b, "#adullam");

    bot_connect(b);

    bot_free(b);
    return 0;
}
