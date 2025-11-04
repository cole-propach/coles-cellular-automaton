# Makefile for SDL2 WASM project

# Compiler and flags
EMCC = emcc
CXXFLAGS = -I./inc -s USE_SDL=2 -O3

# Source files
SRC = main.cpp renderer.cpp

# Output
BUILD_DIR = build
OUT_HTML = $(BUILD_DIR)/index.html
SHELL_FILE = template.html

# Default target: build and run
all: $(OUT_HTML) run

# Build the HTML/WASM
$(OUT_HTML): $(SRC) $(SHELL_FILE) | $(BUILD_DIR)
	$(EMCC) $(SRC) $(CXXFLAGS) -o $(OUT_HTML) --shell-file $(SHELL_FILE)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run with emrun
run: $(OUT_HTML)
	emrun --no_browser --port 8080 $(OUT_HTML)

# Clean build
clean:
	rm -rf $(BUILD_DIR)
