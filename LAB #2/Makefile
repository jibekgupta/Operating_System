CC = gcc
CFLAGS = -Wall -g
TARGET = signal

all: $(TARGET)

$(TARGET): signal.o
	$(CC) $(CFLAGS) -o $(TARGET) signal.o

signal.o: signal.c
	$(CC) $(CFLAGS) -c signal.c

clean:
	rm -f $(TARGET) *.o
