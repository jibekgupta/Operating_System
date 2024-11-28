# Compiler and flags
CC = gcc
CFLAGS = -lpthread -Wall

# Define the executable targets
all: cli serv

# Rule to compile the client program
cli: cli.c
	$(CC) $(CFLAGS) -o client cli.c

# Rule to compile the server program
serv: serv.c list.c
	$(CC) $(CFLAGS) -o server serv.c list.c

# Clean up build files
clean:
	rm -f client server
