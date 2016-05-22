CC=gcc
CFLAGS=-I.

hellomake: parser.o server.o client.o main.o
	$(CC) -o chat parser.o server.o client.o main.o 
	
clean:
	rm *.o chat
