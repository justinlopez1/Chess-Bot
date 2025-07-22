# Makefile to drive CMake

BUILD_DIR := build

.PHONY: all clean run

all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && cmake --build .

clean:
	@rm -rf $(BUILD_DIR)

run: all
	@./$(BUILD_DIR)/chessbot
