CC=g++
CFLAGS=-O3 -s

all: main

main: refactor compiling

refactor:
	python .\src\refactor.py .\src\execute-me.c

compiling:
	$(CC) $(CFLAGS) .\src\exe-archivator.cpp -o exe-archivator.exe