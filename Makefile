# =====================================================
# MemoraDB - In-Memory Database System
# =====================================================
# 
# File                      : Makefile
# Module                    : Build System Configuration
# Last Updating Author      : kei077
# Last Update               : 08/14/2025
# Version                   : 1.0.0
# 
# Description:
#  Build configuration for MemoraDB project including client, server,
#  and comprehensive test suite compilation and execution.
# 
# Targets:
#  all        - Build client and server executables
#  test       - Compile all test files in tests/ directory
#  run-tests  - Compile and execute all tests with colored output
#  clean      - Remove all generated binaries and executables
# 
# Copyright (c) 2025 MemoraDB Project
# =====================================================

# === Compiler and compilation flags === #
CC = gcc
CFLAGS = -Wall -Wextra -I./src
LDFLAGS = -lpthread

# === Source files === #
FILES = $(filter-out $(CLIENT_SRC) $(SERVER_SRC), $(wildcard src/**/*.c))
CLIENT_SRC = src/client/client.c
SERVER_SRC = src/server/server.c

CLIENT_OUT = client
SERVER_OUT = server

# === Test outputs === #
# ============================================================================================ #
# = The patsubst function transforms tests/test_parser.c → tests/test_parser, giving exactly = #
# =                the executable names that get created during compilation                  = #
# ============================================================================================ #

TEST_OUTS = $(patsubst tests/%.c, tests/%,$(wildcard tests/*.c))

# === Targets === #
.PHONY: all clean test run-tests

all: $(CLIENT_OUT) $(SERVER_OUT)

$(CLIENT_OUT): $(CLIENT_SRC) $(FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER_OUT): $(SERVER_SRC) $(FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ======================================================================================== #
# ======================================================================================== #

# === Run the server and compiles all .c files in tests / directory === #
test: $(SERVER_OUT)
	@for test_file in tests/*.c; do \
		test_name=$$(basename $$test_file .c); \
		if [ "$$test_name" = "test_framework" ]; then \
			continue; \
		fi; \
		echo "Compiling $$test_name..."; \
		if [ "$$test_name" = "test_ping_echo" ]; then \
			$(CC) $(CFLAGS) -DTESTING -o tests/$$test_name $$test_file tests/test_framework.c $(SERVER_SRC) $(FILES) $(LDFLAGS); \
		else \
			$(CC) $(CFLAGS) -o tests/$$test_name $$test_file tests/test_framework.c $(FILES) $(LDFLAGS); \
		fi; \
	done

# === Compile and run all tests === #
run-tests: test
	@echo ""
	@echo "╔══════════════════════════════════════════════════════════════════════════╗"
	@echo "║                      MemoraDB Test Suite Runner                          ║"
	@echo "╚══════════════════════════════════════════════════════════════════════════╝"
	@echo ""
	@rm -f /tmp/memoradb_test_results.txt
	@overall_status=0; \
	for test_file in tests/*.c; do \
		test_name=$$(basename $$test_file .c); \
		if [ "$$test_name" = "test_framework" ]; then \
			continue; \
		fi; \
		echo ""; \
		if ! ./tests/$$test_name; then \
			overall_status=1; \
		fi; \
	done; \
	echo ""; \
	echo '#include "test_framework.h"' > /tmp/summary.c; \
	echo 'int main() { print_final_summary(); return 0; }' >> /tmp/summary.c; \
	$(CC) $(CFLAGS) -I./tests -o /tmp/summary /tmp/summary.c tests/test_framework.c $(LDFLAGS); \
	/tmp/summary; \
	rm -f /tmp/summary /tmp/summary.c; \
	exit $$overall_status

# === Clean up generated files === #
clean:
	rm -f $(CLIENT_OUT) $(SERVER_OUT) $(TEST_OUTS)