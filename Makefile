CC=g++
CFLAGS=-O3 -s
all: main

main:
	$(CC) $(CFLAGS) exe-archivator.cpp -o exe-archivator
