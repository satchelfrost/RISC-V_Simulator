#include <iostream>

/* Sample instructions to make sure each instruction works properly */ 

void RV32I::testInstr(int test)
{
	switch (test) {
	case 0:
		// lb x5, 13(x3)
		memory[0]  = 0b10000011;
		memory[1]  = 0b10000010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =          9;
		memory[22] =       -125;
		break;
	case 1:
		// lh x5, 13(x3)
		memory[0]  = 0b10000011;
		memory[1]  = 0b10010010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =          9;
		memory[22] =         42;   
		memory[23] =       -124;
		break;
	case 2:
		// lw x5, 13(x3)
		memory[0]  = 0b10000011;
		memory[1]  = 0b10100010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =          9;
		memory[22] =         42;
		memory[23] =         43;
		memory[24] =         44;
		memory[25] =       -123;
		break;
	case 3:
		// lb x5, 13(x3)
		memory[0]  = 0b10000011;
		memory[1]  = 0b11000010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =          9;
		memory[22] =       -125;
		break;
	case 4:
		// lh x5, 13(x3)
		memory[0]  = 0b10000011;
		memory[1]  = 0b11010010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =          9;
		memory[22] =         42;
		memory[23] =       -124;
		break;
	case 5:
		// lw x5, 13(x3)
		memory[0]  = 0b10000011;
		memory[1]  = 0b11100010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =          9;
		memory[22] =         42;
		memory[23] =         43;
		memory[24] =         44;
		memory[25] =       -123;
		break;
	case 6:
		// sb x5, 173(x3)
		memory[0]   = 0b10100011;
		memory[1]   = 0b10000110;
		memory[2]   = 0b01010001;
		memory[3]   = 0b00001010;
		regs[3]     =          9;
		regs[5]     =       1234;
		memory[182] =         -1;
		break;
	case 7:
		// sh x5, 173(x3)
		memory[0]   = 0b10100011;
		memory[1]   = 0b10010110;
		memory[2]   = 0b01010001;
		memory[3]   = 0b00001010;
		regs[3]     =          9;
		regs[5]     =       1234;
		memory[182] =         -1;
		memory[183] =         -2;
		break;
	case 8:
		// sw x5, 173(x3)
		memory[0]   = 0b10100011;
		memory[1]   = 0b10100110;
		memory[2]   = 0b01010001;
		memory[3]   = 0b00001010;
		regs[3]     =          9;
		regs[5]     =       1234;
		memory[182] =         -1;
		memory[183] =         -2;
		memory[184] =         -3;
		memory[185] =         -4;
		break;
	case 9: 
		// addi x5, x3, 13
		memory[0]  = 0b10010011;
		memory[1]  = 0b10000010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =          9;
		break;
	case 10:
		// stli x5, x3, 13
		memory[0]  = 0b10010011;
		memory[1]  = 0b10100010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =        -13;
		break;
	case 11:
		// stliu x5, x3, 13
		memory[0]  = 0b10010011;
		memory[1]  = 0b10110010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =        -13;
		break;
	case 12:
		// xori x5, x3, 13
		memory[0]  = 0b10010011;
		memory[1]  = 0b11000010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =         65;
		break;
	case 13:
		// ori x5, x3, 13
		memory[0]  = 0b10010011;
		memory[1]  = 0b11100010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =         65;
		break;
	case 14:
		// andi x5, x3, 13
		memory[0]  = 0b10010011;
		memory[1]  = 0b11110010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =         4;
		break;
	case 15:
		// slli x5, x3, 4
		memory[0]  = 0b10010011;
		memory[1]  = 0b10010010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =         17;
		break;
	case 16:
		// srli x5, x3, 4
		memory[0]  = 0b10010011;
		memory[1]  = 0b11010010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =       -128;
		break;
	case 17:
		// srai x5, x3, 4
		memory[0]  = 0b10010011;
		memory[1]  = 0b11010010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b01000000;
		regs[3]    =       -128;
		break;
	case 18:
		// add x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b10000010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =       -128;
		regs[4]    =        130;
		break;
	case 19:
		// sub x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b10000010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b01000000;
		regs[3]    =       -128;
		regs[4]    =        130;
		break;
	case 20:
		// sll x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b10010010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =       -128;
		regs[4]    =          2;
		break;
	case 21:
		// slt x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b10100010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =       -128;
		regs[4]    =          2;
		break;
	case 22:
		// sltu x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b10110010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =       -128;
		regs[4]    =          2;
		break;
	case 23:
		// xor x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b11000010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =          5;
		regs[4]    =          4;
		break;
	case 24:
		// srl x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b11010010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =       -128;
		regs[4]    =          2;
		break;
	case 25:
		// sra x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b11010010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b01000000;
		regs[3]    =       -128;
		regs[4]    =          2;
		break;
	case 26:
		// or x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b11100010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =          5;
		regs[4]    =          4;
		break;
	case 27:
		// and x5, x3, x4
		memory[0]  = 0b10110011;
		memory[1]  = 0b11110010;
		memory[2]  = 0b01000001;
		memory[3]  = 0b00000000;
		regs[3]    =          5;
		regs[4]    =          4;
		break;
	case 28:
		// lui x5, 786439 
		memory[0]  = 0b10110111;
		memory[1]  = 0b01110010;
		memory[2]  = 0b00000000;
		memory[3]  = 0b11000000;
		break;
	case 29:
		// auipc x5, 786439 
		memory[0]  = 0b10010111;
		memory[1]  = 0b01110010;
		memory[2]  = 0b00000000;
		memory[3]  = 0b11000000;
		break;
	case 30:
		// jal x5, 786439 
		memory[0]  = 0b11101111;
		memory[1]  = 0b01110010;
		memory[2]  = 0b00000000;
		memory[3]  = 0b11000000;
		break;
	case 31:
		// jalr x5, -1024 
		memory[0]  = 0b11100111;
		memory[1]  = 0b10000010;
		memory[2]  = 0b00000001;
		memory[3]  = 0b11000000;
		regs[3]    = 3;
		break;
	case 32:
		// beq x3, x4, 5 
		memory[0]  = 0b11100011;
		memory[1]  = 0b10000010;
		memory[2]  = 0b01110001;
		memory[3]  = 0b00000000;
		regs[3]    = 6;
		regs[7]    = 6;
		break;
	case 33:
		// bne x3, x4, 5 
		memory[0]  = 0b11100011;
		memory[1]  = 0b10010010;
		memory[2]  = 0b01110001;
		memory[3]  = 0b00000000;
		regs[3]    = 6;
		regs[7]    = 6;
		break;
	case 34:
		// blt x3, x4, 5 
		memory[0]  = 0b11100011;
		memory[1]  = 0b11000010;
		memory[2]  = 0b01110001;
		memory[3]  = 0b00000000;
		regs[3]    = -6;
		regs[7]    = 6;
		break;
	case 35:
		// bge x3, x4, 5 
		memory[0]  = 0b11100011;
		memory[1]  = 0b11010010;
		memory[2]  = 0b01110001;
		memory[3]  = 0b00000000;
		regs[3]    = 6;
		regs[7]    = -6;
		break;
	case 36:
		// bltu x3, x4, 5 
		memory[0]  = 0b11100011;
		memory[1]  = 0b11100010;
		memory[2]  = 0b01110001;
		memory[3]  = 0b00000000;
		regs[3]    = -6;
		regs[7]    = 6;
		break;
	case 37:
		// bgeu x3, x4, 5 
		memory[0]  = 0b11100011;
		memory[1]  = 0b11110010;
		memory[2]  = 0b01110001;
		memory[3]  = 0b00000000;
		regs[3]    = 6;
		regs[7]    = -6;
		break;
	default:
		std::cout << "No such test" << std::endl;
	}
}
