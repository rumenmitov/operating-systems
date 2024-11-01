CC    := gcc
OBJ   := src/*.c lib/*.c
FLAGS := -Wall -I include


all: test clean release

test:
	@echo "Setting-up tests..."
	@mkdir -p tmp
	@make debug

	@echo "Running test1:"
	@./build/debug/xargs < tests/test1.in > tmp/test1.out
	diff tests/test1.out tmp/test1.out

	@echo "Running test2:"
	@./build/debug/xargs < tests/test2.in > tmp/test2.out
	diff tests/test2.out tmp/test2.out

	@echo "Running test3:"
	@./build/debug/xargs -t < tests/test3.in > tmp/test3.out
	diff tests/test3.out tmp/test3.out

	@echo "Running test4:"
	@./build/debug/xargs -n 3 < tests/test4.in > tmp/test4.out
	diff tests/test4.out tmp/test4.out

	@echo "Running test5:"
	@./build/debug/xargs -n 3 -t < tests/test5.in > tmp/test5.out
	diff tests/test5.out tmp/test5.out

	@echo "Running test6:"
	@./build/debug/xargs -t -n 1 printf "foo-%02d\n" < tests/test6.in > tmp/test6.out
	diff tests/test6.out tmp/test6.out

	@echo "All tests passed!"
	@rm -rf tmp

debug:
	@mkdir -p build/debug
	@$(CC) -o build/debug/xargs $(OBJ) $(FLAGS) -g -DDEBUG

release:
	@mkdir -p build/release
	@$(CC) -o build/release/xargs $(OBJ) $(FLAGS)

clean:
	@rm -rf build **/*~ *~ tmp
