CC=cc
CFLAGS=-std=c99 -Wall -pedantic -ggdb
LDFLAGS=

all: hpl.o noq.o

hpl.o: hpl.c hpl.h
	$(CC) $(CFLAGS) -c hpl.c

noq.o: noq.c noq.h hpl.h
	$(CC) $(CFLAGS) -c noq.c
