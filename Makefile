# === Compiler and compilation flags ===
CC = gcc
CFLAGS = -Wall -Wextra -I./src
LDFLAGS = -lpthread

# === Source files ===
UTILS = src/utils/log.c src/utils/hashTable.c
CLIENT_SRC = src/client/client.c
SERVER_SRC = src/server/server.c

CLIENT_OUT = client
SERVER_OUT = server

# === Targets ===
.PHONY: all clean

all: $(CLIENT_OUT) $(SERVER_OUT)

$(CLIENT_OUT): $(CLIENT_SRC) $(UTILS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER_OUT): $(SERVER_SRC) $(UTILS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(CLIENT_OUT) $(SERVER_OUT)
