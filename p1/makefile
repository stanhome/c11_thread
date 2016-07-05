all:thread

CC=g++
CPPFLAGS=-Wall -std=c++11 -ggdb
LDFLAGS=-pthread

thread:thread.o
	$(CC) $(LDFLAGS) -o $@ $^

thread.o:thread.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $^

.PHONY:
	clean

clean:
	rm thread.o thread
