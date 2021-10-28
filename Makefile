CC = gcc
CFLAGS = -Wall

all: build build/output

build:
	mkdir build

build/output: build/main.o
	$(CC) $(CFLAGS) $^ -o $@

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@