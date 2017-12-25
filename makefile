CC=gcc
CFLAGS=-lvirt -fopenmp -lncurses -fstack-protector

host_connect : host_connect.o
	$(CC) -o host_connect host_connect.c $(CFLAGS)
clean :
	rm host_connect.o
