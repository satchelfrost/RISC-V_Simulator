run:
	g++ -o main src/*.cpp -Werror
	./main 0 1 2 3 4 5 6 
comp:
	g++ -o main src/*.cpp -Werror
debug:
	g++ -g -o main src/*.cpp -Werror
	gdb --args main 0 1 2 3 4 5 6 
