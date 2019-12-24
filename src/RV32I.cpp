#include "RV32I.h"
#include <iostream>

 RV32I::RV32I()
 {
	printInfo = false; 	
 }

RV32I::RV32I(int test)
{
	printInfo = true;
	testInstr(test);
}

bool RV32I::Fetch()
{
	byte0 = memory[PC++];
	byte1 = memory[PC++];
	byte2 = memory[PC++];
	byte3 = memory[PC++];

	u8 opcode = byte0 & 0b01111111;

	switch (opcode) {
	case 0b00000011: load();	break;
	case 0b00001111: fence();	break;
	case 0b00010011: immediate();	break; 
	case 0b01100111: jalr();	break;
	case 0b01110011: e();		break;
	case 0b00010111: aupic();	break;
	case 0b00110111: lui();		break;
	case 0b00100011: store();	break;
	case 0b00110011: regToReg();	break;
	case 0b01100011: branch();	break;
	case 0b01101111: jal();		break;
	default:   error();
	}

	if (running)	return true;
	else		return false;
}

void RV32I::load()
{
	u8 funct3 = (byte1 >> 4) & 0b111;

	switch (funct3) {
	case 0b000: lb();	break;
	case 0b001: lh();	break;
	case 0b010: lw();	break;
	case 0b100: lbu();	break;
	case 0b101: lhu();	break;
	case 0b110: lwu();	break;
	default: error(); 
	}
}

void RV32I::fence()
{
}

void RV32I::immediate()
{
	u8 funct3 = (byte1 >> 4) & 0b111;

	switch (funct3) {
	case 0b000: addi();	break; 
	case 0b001: slli();	break; 
	case 0b010: slti();	break;
	case 0b011: sltiu();	break;
	case 0b100: xori();	break;
	case 0b101: sr();	break;
	case 0b110: ori();	break;
	case 0b111: andi();	break;
	default: error();
	}
}

void RV32I::jalr()
{
}

void RV32I::e()
{
}

void RV32I::aupic()
{
}

void RV32I::lui()
{
}

void RV32I::store()
{
	u8 funct3 = (byte1 >> 4) & 0b111;

	switch (funct3) {
	case 0b000 : sb(); break;
	case 0b001 : sh(); break;
	case 0b010 : sw(); break;
	default: error(); 
	}
}

void RV32I::regToReg()
{
}

void RV32I::branch()
{
}

void RV32I::jal()
{
}

void RV32I::error()
{
	std::cout << "No such instruction!" << std::endl;
	running = false;
}

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
		
		// addi x5, 13(x3)
		memory[0]  = 0b10010011;
		memory[1]  = 0b10000010;
		memory[2]  = 0b11010001;
		memory[3]  = 0b00000000;
		regs[3]    =          9;
		break;
	default:
		std::cout << "No such test" << std::endl;
	}
}

void RV32I::lb()
{
	// obtain bit fields of instruction
	u16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// memory bound error check 
	memBoundErr(rs1, imm);
	
	// load byte and sign extend
	u8 byte = memory[imm + regs[rs1]];
	int signExtended = (int)(signed char) byte;

	// zero register error check
	if (rd == 0) zRegError(signExtended);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tlb rd, imm(rs1)" << std::endl;
		printRegsImm_I(rs1, rd, imm);
		printMemOffset(rs1, 0, byte);
		regs[rd] = signExtended;
		printRdSigned(rd);
	}

	else {
		regs[rd] = signExtended; 
	}

}

void RV32I::lh()
{

	// obtain bit fields of instruction
	u16 imm = getImmed();	
	u8 rs1  = getRs1();
	u8 rd   = getRd();

	// error checks for I-format
	memBoundErr(rs1, imm);

	// load bytes from mem.
	u8 msb  = memory[regs[rs1] + imm + 1];
	u8 lsb  = memory[regs[rs1] + imm + 0];

	// construct half word from bytes
	u16 halfWord  = msb << 8;
	halfWord     |= lsb << 0; 

	// sign extend half word
	int signExtended =  (int)(signed short) halfWord;

	// zero register error check
	if (rd == 0) zRegError(signExtended);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tlh rd, imm(rs1)" << std::endl;
		printRegsImm_I(rs1, rd, imm);
		printMemOffset(rs1, 0, lsb);
		printMemOffset(rs1, 1, msb);
		regs[rd] = signExtended;
		printRdSigned(rd);	
	}

	else {
		regs[rd] = signExtended;
	}
}

void RV32I::lw()
{
	// obtain bit fields of instruction
	u16 imm	= getImmed();	
	u8 rs1	= getRs1();
	u8 rd	= getRd();

	// error checks for I-format
	memBoundErr(rs1, imm);
	
	// load bytes from mem.
	u8 msb    = memory[imm + regs[rs1] + 3];
	u8 secMsb = memory[imm + regs[rs1] + 2];
	u8 secLsb = memory[imm + regs[rs1] + 1];
	u8 lsb    = memory[imm + regs[rs1] + 0];

	// construct word from bytes
	u32 word  = msb    << 24;
	word     |= secMsb << 16;
	word     |= secLsb <<  8;
	word     |= lsb    <<  0;

	// sign extend word
	int signExtended =  (int) word;

	// zero register error check
	if (rd == 0) zRegError(signExtended);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tlw rd, imm(rs1)" << std::endl;
		printRegsImm_I(rs1, rd, imm);
		printMemOffset(rs1, 0, lsb);
		printMemOffset(rs1, 1, secLsb);
		printMemOffset(rs1, 2, secMsb);
		printMemOffset(rs1, 3, msb);
		regs[rd] = signExtended;
		printRdSigned(rd);
	}

	else {
		regs[rd] = signExtended;
	}
}

void RV32I::lbu()
{
	// obtain bit fields of instruction
	u16 imm = getImmed();	
	u8 rs1	= getRs1();
	u8 rd	= getRd();

	// error checks for I-format
	memBoundErr(rs1, imm);

	// load byte from mem.
	u8 byte	= memory[imm + regs[rs1]];

	// zero register error check
	if (rd == 0) zRegError((int) byte);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tlbu rd, imm(rs1)" << std::endl;
		printRegsImm_I(rs1, rd, imm);
		printMemOffset(rs1, 0, byte);
		regs[rd] = byte; 
		printRdUsigned(rd);
	}

	else {
		regs[rd] = byte; 
	}
}


void RV32I::lhu()
{
	// obtain bit fields of instruction
	u16 imm = getImmed();	
	u8 rs1  = getRs1();
	u8 rd   = getRd();

	// error checks for I-format
	memBoundErr(rs1, imm);

	// load bytes from mem.
	u8 msb = memory[regs[rs1] + imm + 1];
	u8 lsb = memory[regs[rs1] + imm + 0];

	// construct half word from bytes
	u16 halfWord  = msb << 8;
	halfWord     |= lsb  << 0; 

	// zero register error check
	if (rd == 0) zRegError((int) halfWord);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tlhu rd, imm(rs1)" << std::endl;
		printRegsImm_I(rs1, rd, imm);
		printMemOffset(rs1, 0, lsb);
		printMemOffset(rs1, 1, msb);
		regs[rd] = halfWord;
		printRdUsigned(rd);
	}

	else {
		regs[rd] = halfWord;
	}
}


void RV32I::lwu()
{
	// obtain bit fields of instruction
	u16 imm = getImmed();	
	u8 rs1  = getRs1();
	u8 rd   = getRd();

	// error checks for I-format
	memBoundErr(rs1, imm);
	
	// load bytes from mem.
	u8 msb     = memory[imm + regs[rs1] + 3];
	u8 secMsb  = memory[imm + regs[rs1] + 2];
	u8 secLsb  = memory[imm + regs[rs1] + 1];
	u8 lsb     = memory[imm + regs[rs1] + 0];

	// construct half word from bytes
	u32 word  = msb    << 24;
	word     |= secMsb << 16;
	word     |= secLsb <<  8;
	word     |= lsb    <<  0;

	// zero register error check
	if (rd == 0) zRegError((int) word);
	
	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tlwu rd, imm(rs1)" << std::endl;
		printRegsImm_I(rs1, rd, imm);
		printMemOffset(rs1, 0, lsb);
		printMemOffset(rs1, 1, secLsb);
		printMemOffset(rs1, 2, secMsb);
		printMemOffset(rs1, 3, msb);
		regs[rd] = word;
		printRdUsigned(rd);
	}

	else {
		regs[rd] = word;
	}
}

void RV32I::sb()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	u16 imm = getSplitImm();

	// error checks for S-format
	memBoundErr(rs1, imm);

	// obtain byte to be stored 
	u8 byte = regs[rs2];

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tsb rs2, imm(rs1)" << std::endl;
		printRegsImm_S(rs1, rs2, imm);
		std::cout << "Before Instruction:\n";
		printMemOffset(rs1, 0, memory[imm + regs[rs1]]);
		memory[imm + regs[rs1]] = byte; 
		std::cout << "After Instruction:\n";
		printMemOffset(rs1, 0, byte);
	}

	else { 
		memory[imm + regs[rs1]] = byte; 
	}
}

void RV32I::sh()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	u16 imm = getSplitImm();

	// error checks for S-format
	memBoundErr(rs1, imm);

	// obtain bytes to be stored 
	u16 halfWord = regs[rs2];
	u8 lsb = halfWord;
	u8 msb = halfWord >> 8;

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tsh rs2, imm(rs1)" << std::endl;
		printRegsImm_S(rs1, rs2, imm);
		std::cout << "Before Instruction:\n";
		printMemOffset(rs1, 0, memory[imm + regs[rs1] + 0]);
		printMemOffset(rs1, 1, memory[imm + regs[rs1] + 1]);
		memory[imm + regs[rs1] + 0] = lsb; 
		memory[imm + regs[rs1] + 1] = msb;
		std::cout << "After Instruction:\n";
		printMemOffset(rs1, 0, memory[imm + regs[rs1] + 0]);
		printMemOffset(rs1, 1, memory[imm + regs[rs1] + 1]);
	}

	else { 
		memory[imm + regs[rs1] + 0] = lsb; 
		memory[imm + regs[rs1] + 1] = msb;
	}
}


void RV32I::sw()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	u16 imm = getSplitImm();

	// error checks for S-format
	memBoundErr(rs1, imm);

	// obtain bytes to be stored 
	u32 word  = regs[rs2];
	u8 lsb    = (word >> 0)  & 0xff;
	u8 secLsb = (word >> 8)  & 0xff;
	u8 secMsb = (word >> 16) & 0xff;
	u8 msb    = (word >> 24) & 0xff;

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tsw rs2, imm(rs1)" << std::endl;
		printRegsImm_S(rs1, rs2, imm);
		std::cout << "Before Instruction:\n";
		printMemOffset(rs1, 0, memory[imm + regs[rs1] + 0]);
		printMemOffset(rs1, 1, memory[imm + regs[rs1] + 1]);
		printMemOffset(rs1, 2, memory[imm + regs[rs1] + 2]);
		printMemOffset(rs1, 3, memory[imm + regs[rs1] + 3]);
		memory[imm + regs[rs1] + 0] = lsb; 
		memory[imm + regs[rs1] + 1] = secLsb; 
		memory[imm + regs[rs1] + 2] = secMsb; 
		memory[imm + regs[rs1] + 3] = msb; 
		std::cout << "After Instruction:\n";
		printMemOffset(rs1, 0, memory[imm + regs[rs1] + 0]);
		printMemOffset(rs1, 1, memory[imm + regs[rs1] + 1]);
		printMemOffset(rs1, 2, memory[imm + regs[rs1] + 2]);
		printMemOffset(rs1, 3, memory[imm + regs[rs1] + 3]);
	}

	else { 
		memory[imm + regs[rs1] + 0] = lsb; 
		memory[imm + regs[rs1] + 1] = secLsb; 
		memory[imm + regs[rs1] + 2] = secMsb; 
		memory[imm + regs[rs1] + 3] = msb; 
	}

}

void RV32I::addi()
{
	// obtain bit fields of instruction
	u16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// error check 
	u32 value = regs[rs1] + imm;
	if (rd == 0) zRegError((int) value);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\taddi rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}

}

void RV32I::slti()
{
	// obtain bit fields of instruction
	u16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// calculate value for slti
	u32 value = ((int) regs[rs1] < imm) ? 1 : 0;

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tslti rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}

}

void RV32I::sltiu()
{

}

void RV32I::xori()
{

}

void RV32I::ori()
{

}

void RV32I::andi()
{

}

void RV32I::slli()
{

}

void RV32I::srli()
{

}

void RV32I::srai()
{

}

void RV32I::sr()
{
	// check bits 25 to 31
	// if 0: srli
	// if 1: srai
	// else error
}

void RV32I::printMach_I()
{
	string b3 = u8Bit(byte3).to_string();
	string b2 = u8Bit(byte2).to_string();
	string b1 = u8Bit(byte1).to_string();
	string b0 = u8Bit(byte0).to_string();
	string machInstr = b3 + b2 + b1 + b0;

	// print out the machine instruction with tabs in between fields
	std::cout << "\t\t\t\timm[11:0]\trs1\tfunct3\trd\topcode\n";
	std::cout << "Machine instruction:\t\t";
	for (auto i = 0; i < machInstr.size(); i++) {
		std::cout << machInstr[i]; 
		if (i == 11 || i == 16 || i == 19 || i == 24)
			std::cout << "\t";
	}
	std::cout << std::endl;
}

void RV32I::printMach_S()
{
	string b3 = u8Bit(byte3).to_string();
	string b2 = u8Bit(byte2).to_string();
	string b1 = u8Bit(byte1).to_string();
	string b0 = u8Bit(byte0).to_string();
	string machInstr = b3 + b2 + b1 + b0;

	// print out the machine instruction with tabs in between fields
	std::cout << "\t\t\t\timm[11:5]\trs2\trs1\tfunct3\timm[4:0]\topcode\n";
	std::cout << "Machine instruction:\t\t";
	for (auto i = 0; i < machInstr.size(); i++) {
		std::cout << machInstr[i]; 
		switch(i) {  
		case  6: std::cout << "\t\t";	break;
		case 11: std::cout << "\t";	break;
		case 16: std::cout << "\t";	break;
		case 19: std::cout << "\t";	break;
		case 24: std::cout << "\t\t";	break;
		}
	}
	std::cout << std::endl;
	
}


void RV32I::printRegsImm_I(u8 rs1, u8 rd, u16 imm)
{
	std::cout << "Source register (rs1):\t\tx" << (int)rs1 << " = "
		<< regs[rs1] << std::endl;
	std::cout << "Destination register (rd):\tx"
		<< (int)rd
		<< " = " << regs[rd] << " (before instruction)" << std::endl;
	std::cout << "offset (imm):\t\t\t" << imm << std::endl;
}

void RV32I::printRegsImm_S(u8 rs1, u8 rs2, u16 imm)
{
	std::cout << "Source register (rs2):\t\tx" << (int)rs2 << " = "
		<< regs[rs2] << std::endl;
	std::cout << "Source reg. bin. (rs2):\t\tx" << (int)rs2 << " = ";
	printBytes(u32Bit(regs[rs2]).to_string());
	std::cout << "Source register (rs1):\t\tx" << (int)rs1 << " = "
		<< regs[rs1] << std::endl;
	std::cout << "Memory offset (imm):\t\t" << imm << std::endl;
	std::cout << "x"
		<< (int)rs1 << " + imm:\t\t\t"
		<< imm + regs[rs1] << std::endl;
	
}

void RV32I::printMemOffset(u8 rs1, int offset, u8 value)
{
	
	std::cout << "memory[x" << (int)rs1 << " + imm + " << offset << "]:\t\t";
	printBytes(u8Bit(value).to_string());
}

void RV32I::printRdSigned(u8 rd)
{
	std::cout << "rd after instruction:\t\t";
	std::cout << "x" << (int)rd << " = " << (int)regs[rd] << "\n";
	std::cout << "rd after instr. binary:\t\t";
	std::cout << "x" << (int)rd << " = "; 
	printBytes(u32Bit(regs[rd]).to_string());
}
void RV32I::printRdUsigned(u8 rd)
{
	std::cout << "rd after instruction:\t\t";
	std::cout << "x" << (int)rd << " = " << regs[rd] << "\n";
	std::cout << "rd after instr. binary:\t\t";
	std::cout << "x" << (int)rd << " = ";
	printBytes(u32Bit(regs[rd]).to_string());
}


void RV32I::zRegError(int value)
{
	if (value != 0) {
		std::cout << "Cannot set zero register to anything other than zero."
			<< std::endl;
		running = false;
	}
}

void RV32I::memBoundErr(u8 rs1, u16 imm)
{
	if (regs[rs1] + imm > MaxMemory - 1) {
		std::cout << "Memory offset (i.e. rs1 + imm.) out of range." << std::endl;
		std::cout << "MaxMemory - 1 = " << MaxMemory - 1 << std::endl;
		std::cout << "regs[rs1] + imm = " << regs[rs1] + imm << std::endl;
		running = false;
	}
}

RV32I::u8 RV32I::getRs1()
{
	u8 rs1 = (byte1 >> 7) & 1;
	rs1 |= ((byte2 << 1) & 0b11110);
	return rs1;
}

RV32I::u8 RV32I::getRs2()
{
	u8 rs2 = (byte3 & 1) << 4;
	rs2   |= (byte2 >> 4);
	return rs2;
}


RV32I::u8 RV32I::getRd()
{
	u8 rd = (byte0 >> 7) & 1;
	rd |= ((byte1 << 1) & 0b11110);
	return rd;	
}

RV32I::u16 RV32I::getSplitImm()
{
	// imm[11:5]
	u8 imm11to5 = byte3 >> 1;

	// imm[4:0]
	u8 imm4to0 = (byte0 >> 7) & 1;
	imm4to0   |= ((byte1 << 1) & 0b11110);

	// construct imm[11:0]
	u16 imm  = imm11to5 << 5;
	imm 	|= imm4to0;

	return imm;
}


RV32I::u16 RV32I::getImmed()
{
	u16 imm = (byte2 >> 4) & 0b1111;
	imm |= (byte3 << 4);
	return imm;	
}

void RV32I::printBytes(string bin)
{
	for (auto i = 0; i < bin.size(); i++) {
		if (i == 8 || i == 16 || i == 24) std::cout << " ";
		std::cout << bin[i];
	}
	std::cout << std::endl;
}







