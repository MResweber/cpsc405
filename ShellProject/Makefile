CC = gcc
CFLAGS = -Wall

all: myShell myLs myCat

myShell: starter_sh.c
	$(CC) $(CFLAGS) starter_sh.c -o myShell

myLs: myLs.c
	$(CC) $(CFLAGS) myLs.c -o myLs

myCat: myCat.c
	$(CC) $(CFLAGS) myCat.c -o myCat

clean: 
	rm -f myShell myLs myCat