CC=g++
CFLAGS=-O3 -s
all: main

main:
	python refactor.py
	$(CC) $(CFLAGS) exe-archivator.cpp -o exe-archivator
