CC    := gcc
OBJ   := src/main.c src/mwc.c
FLAGS := -Wall

all: release

debug:
	@mkdir -p build/debug
	@$(CC) -o build/debug/mwc $(OBJ) $(FLAGS) -g -DDEBUG

release:
	@mkdir -p build/release
	@$(CC) -o build/release/mwc $(OBJ) $(FLAGS) -O3

clean:
	@rm -rf build *~ src/*~
