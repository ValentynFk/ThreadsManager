# Makefile for making this project
CC = gcc
CFLAGS = -g
CLIBS = -lpthread

.PHONY: all clean threadsmanager

all: threadsmanager

clean:
	rm -f ./threadsmanager

threadsmanager:
	$(CC) $(CFLAGS) main.c -o $@ $(CLIBS)
