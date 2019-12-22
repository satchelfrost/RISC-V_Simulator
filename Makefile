compRun:
	g++ -o bin/main src/*.cpp -Werror
	./bin/main 6 7 8
comp:
	g++ -o bin/main src/*.cpp -Werror
run:
	./bin/main 6 7 8
debug:
	g++ -g -o bin/debug src/*.cpp -Werror
	gdb --args bin/debug 6 7 8
