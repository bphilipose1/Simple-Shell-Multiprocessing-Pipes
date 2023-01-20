CC=g++
CFLAGS=-c -Wall

all: myshell

myshell: myshell.o
	$(CC) myshell.o -o myshell

myshell.o: myshell.cpp
	$(CC) $(CFLAGS) myshell.cpp

clean:
	rm -f myshell myshell.o
