CC=gcc
CFLAGS=-g -Wall

main: main.o sortingAlgos.o 
	$(CC) $(CFLAGS) -o main main.o sortingAlgos.o
	make clean
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
sortingAlgos.o: sortingAlgos.h sortingAlgos.c
	$(CC) $(CFLAGS) -c sortingAlgos.c

clean:
	del *.o