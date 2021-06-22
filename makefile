CC = gcc
CFLAGS = -g -Wall -std=c99

all : prog

prog : main.o hashMap.o
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.c hashMap.h

hashMap.o : hashMap.h hashMap.c

.PHONY : clean memCheckProg

memCheckProg :
	valgrind --tool=memcheck --leak-check=yes prog

clean :
	-rm *.o
	-rm prog
