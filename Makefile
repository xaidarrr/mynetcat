CC = gcc
CFLAGS = -Wall -pthread -Iinclude

SRC_DIR = src
BUILD_DIR = build

SRC_FILES = $(SRC_DIR)/client.c $(SRC_DIR)/server.c $(SRC_DIR)/main.c

OBJ_FILES = $(BUILD_DIR)/client.o $(BUILD_DIR)/server.o $(BUILD_DIR)/main.o

EXECUTABLE = mync

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
