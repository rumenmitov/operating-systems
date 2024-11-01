CC     := gcc
SRC    := src/*.c
FLAGS  := -Wall


all: release

debug:
	@mkdir -p build/debug
	@$(CC) -o build/debug/vending_machine $(SRC) $(FLAGS) -g -DDEBUG

release:
	@mkdir -p build/release
	@$(CC) -o build/release/vending_machine $(SRC) $(FLAGS) -O3

clean:
	@rm -rf build/ *~ **/*~
