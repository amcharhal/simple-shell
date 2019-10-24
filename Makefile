CFLAGS=-std=c99
header: header.o main.o
	gcc -o header header.o main.o

header.o: header.c
	gcc -o header.o -c header.c -W -Wall

main.o: main.c header.h
	gcc -o main.o -c main.c -W -Wall

clean:
	rm -rf *.o
mrproper: clean
	rm -rf header
