CC    := gcc
SRC   := src/*.c
FLAGS := -Wall

all: release

debug:
	@mkdir -p build/debug
	@$(CC) -o build/debug/prisoners $(SRC) $(FLAGS) -g -DDEBUG

release:
	@mkdir -p build/release
	@$(CC) -o build/release/prisoners $(SRC) $(FLAGS) -O3

clean:
	rm -rf *~ **/*~ build/ 
