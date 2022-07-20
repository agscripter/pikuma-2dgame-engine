# Variables
CC = g++
LANG_STD = c++17
COMPILER_FLAGS = -Wall -Wfatal-errors -std=$(LANG_STD)
INCLUDE_PATH = "./libs/"
SOURCE_FILES = src/*.cpp src/**/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.4
OUTPUT_BIN = gameengine

# Rules
build:
	$(CC) $(COMPILER_FLAGS) -I$(INCLUDE_PATH) $(SOURCE_FILES) $(LINKER_FLAGS) -o $(OUTPUT_BIN)

run:
	./$(OUTPUT_BIN)

clean:
	rm $(OUTPUT_BIN)
