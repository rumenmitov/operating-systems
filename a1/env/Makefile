CC    := gcc
SRC   := src/*.c
FLAGS := -Wall

all: release

debug:
	@mkdir -p build/debug
	@$(CC) -o build/debug/env $(SRC) $(FLAGS) -DDEBUG -g

release:
	@mkdir -p build/release
	@$(CC) -o build/release/env $(SRC) $(FLAGS)

clean:
	@rm -rf **/*~
	@rm -rf build/
