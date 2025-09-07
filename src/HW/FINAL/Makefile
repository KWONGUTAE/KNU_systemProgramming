CC = gcc
CFLAGS = -Wall -pthread
TARGETS = server client

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o server server.c -lpthread

client: client.c
	$(CC) $(CFLAGS) -o client client.c -lpthread

clean:
	rm -f $(TARGETS)
