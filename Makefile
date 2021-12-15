CC=g++
CFLAGS=-O3 -s
all: main

main:
	python .\src\refactor.py
	$(CC) $(CFLAGS) .\src\exe-archivator.cpp -o exe-archivator.exe