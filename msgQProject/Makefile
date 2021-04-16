CC = gcc
CFLAGS = -g -Wall -pthread

defualt: msgqProject

msgqProject: main.o msgq.o
	$(CC) $(CFLAGS) -o msgqProject main.o msgq.o

main.o: main.c msgq.h
	$(CC) $(CFLAGS) -c main.c

msgq.o: msgq.c msgq.h
	$(CC) $(CFLAGS) -c msgq.c

clean: 
	rm -f msgqProject *.0