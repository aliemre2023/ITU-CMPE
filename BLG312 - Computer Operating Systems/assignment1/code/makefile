BIN_DIR = bin
INC_DIR = include
SRC_DIR = src

PROCESS_O = process
JOB_O = job

CFLAGS = -Wall -Wextra -I./$(INC_DIR)

build:
	gcc $(CFLAGS) $(SRC_DIR)/$(PROCESS_O).c -o $(BIN_DIR)/$(PROCESS_O)
	gcc $(CFLAGS) $(SRC_DIR)/$(JOB_O).c -o $(BIN_DIR)/$(JOB_O)

run:
	./$(BIN_DIR)/$(PROCESS_O)
