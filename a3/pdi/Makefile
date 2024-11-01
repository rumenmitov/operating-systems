CC      := gcc
SRC     := src/*.c
FLAGS   := -Wall


all: release

release:
	@mkdir -p ./build/release
	@$(CC) -o ./build/release/pdi-numbers $(SRC) $(FLAGS)

graph: release
	@./graph.sh
	@gnuplot --version
	@gnuplot -p config.plt

debug:
	@mkdir -p ./build/debug
	@$(CC) -o ./build/debug/pdi-numbers $(SRC) $(FLAGS) -g -DDEBUG

clean:
	@rm -rf build/ *~ **/*~ results/
