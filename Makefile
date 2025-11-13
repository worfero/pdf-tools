SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include

CC = g++
CFLAGS = -Wall -I$(INCLUDE_DIR)

SRCS = $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = $(OBJ_DIR)/pdf-tools

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	@echo "Running $(TARGET)..."
	@$(TARGET)