current:
	g++ -o bin/main src/*.cpp -Werror
	./bin/main 27 28 29
all:
	g++ -o bin/main src/*.cpp -Werror
	./bin/main 0 1 2 3 4 5 6 7 8 9 10
comp:
	g++ -o bin/main src/*.cpp -Werror
run:
	./bin/main 0 1 2 3 4 5 6 7 8
debug:
	g++ -g -o bin/debug src/*.cpp -Werror
	gdb --args bin/debug 0 1 2 3 4 5 6 7 8
