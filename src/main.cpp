#include <iostream>
#include "RV32I.h"

void printAvailableTests();

int main(int argc, char** argv) 
{
	if (argc == 1) printAvailableTests();
	
	// run through the suite of tests 1, 2, etc...
	for (int i = 1; i < argc; i++) {
		int test = std::stoi(argv[i]);
		RV32I ISA(test);
		ISA.Fetch();
		std::cout << "\n" << std::endl;
	}
}

void printAvailableTests()
{
	std::cout << "\nAvailable Tests:\n";
	std::cout << "0. Load byte\n";
	std::cout << "1. Load half word\n";
	std::cout << "2. Load word\n";
	std::cout << "3. Load byte unsigned\n";
	std::cout << "4. Load half word unsiged\n";
	std::cout << "5. Load word unsigned\n";
	std::cout << "6. Store byte\n";
	std::cout << "7. Store half\n";
	std::cout << "8. Store word\n";
	std::cout << "\ne.g. to run tests 3, 4, and 5 --> ./main 3 4 5\n";
}
