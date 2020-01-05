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
	std::cout << "e.g. to run avaiable tests 3, 4, and 5 --> ./main 3 4 5\n\n";
	std::cout << "\nAvailable Tests:\n";
	std::cout << "0.  Load byte\n";
	std::cout << "1.  Load half word\n";
	std::cout << "2.  Load word\n";
	std::cout << "3.  Load byte unsigned\n";
	std::cout << "4.  Load half word unsiged\n";
	std::cout << "5.  Load word unsigned\n";
	std::cout << "6.  Store byte\n";
	std::cout << "7.  Store half\n";
	std::cout << "8.  Store word\n";
	std::cout << "9.  Add immediate\n";
	std::cout << "10. Set less than immediate\n";
	std::cout << "11. Set less than immediate unsigned\n";
	std::cout << "12. Exclusive or immediate\n";
	std::cout << "13. Or immediate\n";
	std::cout << "14. And immediate\n";
	std::cout << "15. Shift left logical immediate\n";
	std::cout << "16. Shift right logical immediate\n";
	std::cout << "17. Shift right arithmetic immediate\n";
	std::cout << "18. Add\n";
	std::cout << "19. Sub\n";
	std::cout << "20. Shift left logical\n";
	std::cout << "21. Set less than\n";
	std::cout << "22. Set less than unsigned\n";
	std::cout << "23. Exclusive or\n";
	std::cout << "24. Shift right logical\n";
	std::cout << "25. Shift right arithmetic\n";
	std::cout << "26. Or\n";
	std::cout << "27. And\n";
	std::cout << "28. Load upper immediate\n";
	std::cout << "29. Add upper immediate to program counter\n";
	std::cout << "30. Jump and link\n";
	std::cout << "31. Jump and link register\n";
	std::cout << "32. Branch if equal\n";
	std::cout << "33. Branch if not equal\n";
	std::cout << "34. Branch less than\n";
	std::cout << "35. Branch greater than\n";
	std::cout << "36. Branch less than unsigned\n";
	std::cout << "37. Branch greater than unsigned\n";
}
