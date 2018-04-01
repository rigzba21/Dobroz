CC=gcc
CFLAGS=-Wall -lvirt -fopenmp -lmenu -lncurses -fstack-protector -flto -D_FORTIFY_SOURCE=2

host_connect : host_connect.o
	$(CC) -o host_connect host_connect.c $(CFLAGS)
clean :
	rm host_connect.o
