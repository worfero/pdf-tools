SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include

CC = g++
CFLAGS = -Wall -I$(INCLUDE_DIR)

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TARGET = $(OBJ_DIR)/pdf-lib

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	@echo "Running $(TARGET)..."
	@$(TARGET)