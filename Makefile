HEADERS = client.h

default: program

shell.o: client.c $(HEADERS)
	gcc -c client.c -o client.o

program: client.o
	gcc client.o -o client

clean:
	-rm -f client.o
	-rm -f client
