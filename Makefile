# === Compiler and compilation flags ===
CC = gcc
CFLAGS = -Wall -Wextra -I./src
LDFLAGS = -lpthread

# === Source files ===
FILES = src/utils/log.c src/utils/hashTable.c src/parser/parser.c src/utils/logo.c
CLIENT_SRC = src/client/client.c
SERVER_SRC = src/server/server.c

CLIENT_OUT = client
SERVER_OUT = server

# === Test files ===
TESTS = tests/test_ping_echo.c
TEST_OUT = test_ping_echo

# === Targets ===
.PHONY: all clean test

all: $(CLIENT_OUT) $(SERVER_OUT)

$(CLIENT_OUT): $(CLIENT_SRC) $(FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER_OUT): $(SERVER_SRC) $(FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: $(TEST_OUT)

$(TEST_OUT): $(TESTS) $(SERVER_SRC) $(FILES)
	$(CC) $(CFLAGS) -DTESTING -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(CLIENT_OUT) $(SERVER_OUT) $(TEST_OUT)
