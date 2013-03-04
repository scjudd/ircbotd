all:
	gcc -Wall -g -lev tcp.c queue.c bot.c test_bot.c -o test_bot
	gcc -Wall -g queue.c test_queue.c -o test_queue

clean:
	rm test_bot test_queue
