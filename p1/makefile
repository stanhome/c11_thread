all:Thread

CC=g++
CPPFLAGS=-Wall -std=c++11 -ggdb
LDFLAGS=-pthread

Thread:Thread.o
	$(CC) $(LDFLAGS) -o $@ $^

Thread.o:thread.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^

.PHONY:
	clean

clean:
	rm Thread.o Thread
