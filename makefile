# This makefile is intended to build the Threads-Manager solution
# Written by Valentyn Faychuk at SoftServe on 04.12.2019
# email:
# 		faitchouk.valentyn@gmail.com

SHELL = /bin/sh
$(shell mkdir -p build)

ODIR = build
# Include directories
IDIR = .
# Sources
SOURCES = main
# Libs
CLIBS = -lpthread

# Compiler and flags
CC = gcc
CFLAGS = -g -I$(IDIR)

.PHONY: all clean thmanager

all: thmanager

thmanager: setup
	$(CC) $(CFLAGS) -o $(ODIR)/$@ $(shell find $(ODIR)/ -type f -name \*.o) $(CLIBS)

setup:
	for SOURCE in $(SOURCES) ; do \
		$(CC) -c $$SOURCE.c $(CFLAGS) -o $(ODIR)/$$SOURCE.o ; \
	done

clean:
	rm -f $(ODIR)/*.o $(ODIR)/thmanager

