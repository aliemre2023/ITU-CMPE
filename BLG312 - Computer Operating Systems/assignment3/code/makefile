CC = gcc
CFLAGS = -Wall -Wextra -I./include

SRC = src/main.c src/fs.c src/utils.c
OUT = bin/main
MINI_FS = bin/mini_fs

.PHONY: build run check clean

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	./$(OUT)

check: build
	$(CC) $(CFLAGS) src/mini_fs.c src/utils.c -o $(MINI_FS)
	$(MINI_FS) tests/commands.txt tests/output.txt 2>/dev/null

	diff -u tests/expected_output.txt tests/output.txt || { echo "Output mismatch"; exit 1; }
