#include "RV32I.h"
#include "testInstr.h"
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
	case 0b00010111: auipc();	break;
	case 0b00110111: lui();		break;
	case 0b00100011: store();	break;
	case 0b00110011: rFormat();	break;
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
	case 0b110: lwu();	break; // not apart of RV32I
	default: error(); 
	}
}

void RV32I::fence()
{
	// currently missing
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
	case 0b101: sr_I();	break; // two instructions
	case 0b110: ori();	break;
	case 0b111: andi();	break;
	default: error();
	}
}

void RV32I::jalr()
{
	// obtain bit fields of instruction
	u32 imm = getImmed(); 
	u8 rd   = getRd();
	u8 rs1  = getRs1();

	// sign extend if necessary
	if (imm >> 11 == 1)
		imm |= 0b11111111111111111111000000000000;
	
	// calculate value to set register
	u32 value = regs[rs1] + (int) imm;
	u32 after = PC + 4;

	// zero register error check
	if (rd == 0) zRegError((int) value);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_U();
		std::cout << "Assembly:\t\t\tjalr rd, imm" << std::endl;
		printRegsImm_U(rd, imm);
		std::cout << "PC (before):\t\t\t" << PC << std::endl;
		std::cout << "PC (after):\t\t\t" << (int) value << std::endl;
		std::cout << "ra\t\t\t\t" << after << std::endl;
		PC = value;
		regs[rd] = after;
		printRdSigned(rd);
	}

	else {
		PC = value;
		PC &= 0xfffffffe;
		regs[rd] = after;
	}
}

void RV32I::e()
{
	// ecall and ebreak, currently missing
}

void RV32I::auipc()
{
	// obtain bit fields of instruction
	u32 imm = getImm31to12(); 
	u8  rd  =    getRd();

	// calculate value of upper immediate
	u32 upperImm = imm << 12; 

	// calculate value to set register
	u32 value = PC + upperImm;

	// zero register error check
	if (rd == 0) zRegError((int) value);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_U();
		std::cout << "Assembly:\t\t\tauipc rd, imm" << std::endl;
		printRegsImm_U(rd, imm);
		std::cout << "upper imm.\t\t\t" << upperImm << std::endl;
		std::cout << "upper imm. (bin.)\t\t"; 
		printBytes(u32Bit(upperImm).to_string());
		std::cout << "PC:\t\t\t\t" << PC << std::endl;
		std::cout << "PC + upper imm.:\t\t" << value << std::endl;
		regs[rd] = value;
		printRdSigned(rd);
	}

	else {
		regs[rd] = value;
	}
}

void RV32I::lui()
{
	// obtain bit fields of instruction
	u32 imm = getImm31to12(); 
	u8  rd  =    getRd();

	// calculate value of upper immediate
	u32 value = imm << 12; 

	// zero register error check
	if (rd == 0) zRegError((int) value);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_U();
		std::cout << "Assembly:\t\t\tlui rd, imm" << std::endl;
		printRegsImm_U(rd, imm);
		regs[rd] = value;
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
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

void RV32I::rFormat()
{
	u8 funct3 = (byte1 >> 4) & 0b111;

	switch (funct3) {
	case 0b000: addSub();	break; // two instructions
	case 0b001: sll();	break;
	case 0b010: slt();	break;
	case 0b011: sltu();	break;
	case 0b100: xOr();	break;
	case 0b101: sr_R();	break; // two instructions
	case 0b110: Or();	break;
	case 0b111: And();	break;
	default: error();
	}

}

void RV32I::branch()
{
	u8 funct3 = (byte1 >> 4) & 0b111;

	switch (funct3) {
	case 0b000: beq();	break;
	case 0b001: bne();	break;
	case 0b100: blt();	break;
	case 0b101: bge();	break;
	case 0b110: bltu();	break;
	case 0b111: bgeu();	break;
	default: error(); 
	}
	
}

void RV32I::jal()
{
	// obtain bit fields of instruction
	u32 imm = getImm31to12(); 
	u8  rd  =    getRd();

	// sign extend if necessary
	if (imm >> 19 == 1)
		imm |= 0b11111111111100000000000000000000;
	
	// calculate value to set register
	u32 value = (int) PC + (int) imm;
	u32 after = PC + 4;

	// zero register error check
	if (rd == 0) zRegError((int) value);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_U();
		std::cout << "Assembly:\t\t\tjal rd, imm" << std::endl;
		printRegsImm_U(rd, imm);
		std::cout << "PC (before):\t\t\t" << PC << std::endl;
		std::cout << "PC (after):\t\t\t" << (int) value << std::endl;
		std::cout << "ra\t\t\t\t" << after << std::endl;
		PC = value;
		regs[rd] = after;
		printRdSigned(rd);
	}

	else {
		PC = value;
		regs[rd] = after;
	}
}

void RV32I::error()
{
	std::cout << "No such instruction!" << std::endl;
	running = false;
}

void RV32I::lb()
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// memory bound error check 
	memBoundErr(rs1, imm, 1);
	
	// load byte and sign extend
	u8 byte = memory[imm + regs[rs1]];
	int signExtended = (int)(signed char) byte;

	// zero register error check
	if (rd == 0) zRegError(signExtended);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tlb rd, imm(rs1)" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
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
	s16 imm = getImmed();	
	u8 rs1  = getRs1();
	u8 rd   = getRd();

	// error checks for I-format
	memBoundErr(rs1, imm, 2);

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
		printRegsImm_I(rs1, rd, imm, true);
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
	s16 imm	= getImmed();	
	u8 rs1	= getRs1();
	u8 rd	= getRd();

	// error checks for I-format
	memBoundErr(rs1, imm, 4);
	
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
		printRegsImm_I(rs1, rd, imm, true);
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
	s16 imm = getImmed();	
	u8 rs1	= getRs1();
	u8 rd	= getRd();

	// error checks for I-format
	memBoundErr(rs1, imm, 1);

	// load byte from mem.
	u8 byte	= memory[imm + regs[rs1]];

	// zero register error check
	if (rd == 0) zRegError((int) byte);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tlbu rd, imm(rs1)" << std::endl;
		printRegsImm_I(rs1, rd, imm, false);
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
	s16 imm = getImmed();	
	u8 rs1  = getRs1();
	u8 rd   = getRd();

	// error checks for I-format
	memBoundErr(rs1, imm, 2);

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
		printRegsImm_I(rs1, rd, imm, false);
		printMemOffset(rs1, 0, lsb);
		printMemOffset(rs1, 1, msb);
		regs[rd] = halfWord;
		printRdUsigned(rd);
	}

	else {
		regs[rd] = halfWord;
	}
}


void RV32I::lwu() // not apart of RV32I
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	u8 rs1  = getRs1();
	u8 rd   = getRd();

	// error checks for I-format
	memBoundErr(rs1, imm, 4);
	
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
		printRegsImm_I(rs1, rd, imm, false);
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
	s16 imm = getSplitImm();

	// error checks for S-format
	memBoundErr(rs1, imm, 1);

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
	s16 imm = getSplitImm();

	// error checks for S-format
	memBoundErr(rs1, imm, 2);

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
	s16 imm = getSplitImm();

	// error checks for S-format
	memBoundErr(rs1, imm, 4);

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
	s16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// error check 
	u32 value = regs[rs1] + imm;
	if (rd == 0) zRegError((int) value);

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\taddi rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
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
	s16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// calculate value for slti
	u32 value = ((int) regs[rs1] < (int) imm) ? 1 : 0;

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tslti rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::sltiu()
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// calculate value for slti
	u32 value = (regs[rs1] < imm) ? 1 : 0;

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tslti rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, false);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::xori()
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// calculate value for xori
	u32 value = regs[rs1] ^ imm;

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\txori rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::ori()
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// calculate value for xori
	u32 value = regs[rs1] | imm;

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tori rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::andi()
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	u8  rs1 =   getRs1();
	u8  rd  =    getRd();

	// calculate value for andi
	u32 value = regs[rs1] & imm;

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tandi rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::slli()
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	imm    &= 0b11111;
	u8  rs1 = getRs1();
	u8  rd  = getRd();

	// calculate value for slli
	u32 value = regs[rs1] << imm; 

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tslli rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::srli()
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	imm    &= 0b11111;
	u8  rs1 = getRs1();
	u8  rd  = getRd();

	// calculate value for srli
	u32 value = regs[rs1] >> imm; 

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tsrli rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::srai()
{
	// obtain bit fields of instruction
	s16 imm = getImmed();	
	imm    &= 0b11111;
	u8  rs1 = getRs1();
	u8  rd  = getRd();

	// calculate value for srai
	u32 value = (int) regs[rs1] >> imm; 

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_I();
		std::cout << "Assembly:\t\t\tsrai rd, rs1, imm" << std::endl;
		printRegsImm_I(rs1, rd, imm, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::sr_I()
{
	u8 srOpcode = get31to25();
	switch (srOpcode) {
	case 0b0000000: srli(); break;
	case 0b0100000: srai(); break;
	default: error();
	}
}

void RV32I::add()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for srai
	u32 value = (int) regs[rs1] + (int) regs[rs2];

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tadd rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::sub()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for srai
	u32 value = (int) regs[rs1] - (int) regs[rs2];

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tadd rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::addSub()
{
	u8 srOpcode = get31to25();
	switch (srOpcode) {
	case 0b0000000: add(); break;
	case 0b0100000: sub(); break;
	default: error();
	}
}

void RV32I::sll()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for srai
	u32 value = regs[rs1] << regs[rs2];

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tsll rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::slt()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for slt
	u32 value = ((int) regs[rs1] < (int) regs[rs2]) ? 1 : 0;

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tslt rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, true);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::sltu()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for sltu
	u32 value = (regs[rs1] < regs[rs2]) ? 1 : 0;

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tsltu rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, false);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::xOr()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for xor
	u32 value = regs[rs1] ^ regs[rs2];

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\txor rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, false);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::srl()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for srl
	u32 value = regs[rs1] >> regs[rs2]; 

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tsrl rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, false);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}

}

void RV32I::sra()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for srai
	u32 value = (int) regs[rs1] >> regs[rs2]; 

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tsra rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, false);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}

}

void RV32I::sr_R()
{
	u8 srOpcode = get31to25();
	switch (srOpcode) {
	case 0b0000000: srl(); break;
	case 0b0100000: sra(); break;
	default: error();
	}
}

void RV32I::Or()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for or
	u32 value = regs[rs1] | regs[rs2];

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tor rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, false);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::And()
{
	// obtain bit fields of instruction
	u8  rs1 = getRs1();
	u8  rs2 = getRs2();
	u8  rd  = getRd();

	// calculate value for andi
	u32 value = regs[rs1] & regs[rs2];

	// zero register error check 
	if (rd == 0) zRegError((int) value);	

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_R();
		std::cout << "Assembly:\t\t\tand rd, rs1, rs2" << std::endl;
		printRegsImm_R(rs1, rs2, rd, false);
		regs[rd] = value; 
		printRdSigned(rd);
	}

	else {
		regs[rd] = value; 
	}
}

void RV32I::beq()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	s16 imm = getSplitImm();

	// sign extend if necessary
	if (imm >> 11 == 1)
		imm |= 0b11111111111100000000000000000000;

	// calculate value to set register
	u32 bta = (int) PC + (int) imm;

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tbeq rs1, rs2, imm" << std::endl;
		printRegsImm_B(rs1, rs2, imm, true);
		std::cout << "PC (before):\t\t\t" << PC << std::endl;
		std::cout << "branch target addr.:\t\t" << bta << std::endl;
		
		if (regs[rs1] == regs[rs2])
			PC = bta;

		std::cout << "PC (after):\t\t\t" << (int) PC << std::endl;
	}

	else {
		if (regs[rs1] == regs[rs2])
			PC = bta;
	}
}

void RV32I::bne()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	s16 imm = getSplitImm();

	// sign extend if necessary
	if (imm >> 11 == 1)
		imm |= 0b11111111111100000000000000000000;

	// calculate value to set register
	u32 bta = (int) PC + (int) imm;

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tbne rs1, rs2, imm" << std::endl;
		printRegsImm_B(rs1, rs2, imm, true);
		std::cout << "PC (before):\t\t\t" << PC << std::endl;
		std::cout << "branch target addr.:\t\t" << bta << std::endl;
		
		if (regs[rs1] != regs[rs2])
			PC = bta;

		std::cout << "PC (after):\t\t\t" << (int) PC << std::endl;
	}

	else {
		if (regs[rs1] != regs[rs2])
			PC = bta;
	}

}

void RV32I::blt()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	s16 imm = getSplitImm();

	// sign extend if necessary
	if (imm >> 11 == 1)
		imm |= 0b11111111111100000000000000000000;

	// calculate value to set register
	u32 bta = (int) PC + (int) imm;

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tblt rs1, rs2, imm" << std::endl;
		printRegsImm_B(rs1, rs2, imm, true);
		std::cout << "PC (before):\t\t\t" << PC << std::endl;
		std::cout << "branch target addr.:\t\t" << bta << std::endl;
		
		if ((int) regs[rs1] < (int) regs[rs2])
			PC = bta;

		std::cout << "PC (after):\t\t\t" << (int) PC << std::endl;
	}

	else {
		if ((int) regs[rs1] < regs[rs2])
			PC = bta;
	}

}

void RV32I::bge()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	s16 imm = getSplitImm();

	// sign extend if necessary
	if (imm >> 11 == 1)
		imm |= 0b11111111111100000000000000000000;

	// calculate value to set register
	u32 bta = (int) PC + (int) imm;

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tbge rs1, rs2, imm" << std::endl;
		printRegsImm_B(rs1, rs2, imm, true);
		std::cout << "PC (before):\t\t\t" << PC << std::endl;
		std::cout << "branch target addr.:\t\t" << bta << std::endl;
		
		if ((int) regs[rs1] >= (int) regs[rs2])
			PC = bta;

		std::cout << "PC (after):\t\t\t" << (int) PC << std::endl;
	}

	else {
		if ((int) regs[rs1] == regs[rs2])
			PC = bta;
	}

}

void RV32I::bltu()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	s16 imm = getSplitImm();

	// sign extend if necessary
	if (imm >> 11 == 1)
		imm |= 0b11111111111100000000000000000000;

	// calculate value to set register
	u32 bta = (int) PC + (int) imm;

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tbltu rs1, rs2, imm" << std::endl;
		printRegsImm_B(rs1, rs2, imm, false);
		std::cout << "PC (before):\t\t\t" << PC << std::endl;
		std::cout << "branch target addr.:\t\t" << bta << std::endl;
		
		if (regs[rs1] < regs[rs2])
			PC = bta;

		std::cout << "PC (after):\t\t\t" << (int) PC << std::endl;
	}

	else {
		if (regs[rs1] < regs[rs2])
			PC = bta;
	}

}

void RV32I::bgeu()
{
	// obtain bit fields of instruction
	u8 rs1  = getRs1();
	u8 rs2  = getRs2();
	s16 imm = getSplitImm();

	// sign extend if necessary
	if (imm >> 11 == 1)
		imm |= 0b11111111111100000000000000000000;

	// calculate value to set register
	u32 bta = (int) PC + (int) imm;

	// print reg. states before and after instruction
	if (printInfo) {
		printMach_S();
		std::cout << "Assembly:\t\t\tbgeu rs1, rs2, imm" << std::endl;
		printRegsImm_B(rs1, rs2, imm, false);
		std::cout << "PC (before):\t\t\t" << PC << std::endl;
		std::cout << "branch target addr.:\t\t" << bta << std::endl;
		
		if (regs[rs1] >= regs[rs2])
			PC = bta;

		std::cout << "PC (after):\t\t\t" << (int) PC << std::endl;
	}

	else {
		if (regs[rs1] >= regs[rs2])
			PC = bta;
	}

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

void RV32I::printMach_R()
{
	string b3 = u8Bit(byte3).to_string();
	string b2 = u8Bit(byte2).to_string();
	string b1 = u8Bit(byte1).to_string();
	string b0 = u8Bit(byte0).to_string();
	string machInstr = b3 + b2 + b1 + b0;

	// print out the machine instruction with tabs in between fields
	std::cout << "\t\t\t\t[31:25]\trs2\trs1\tfunct3\trd\topcode\n";
	std::cout << "Machine instruction:\t\t";
	for (auto i = 0; i < machInstr.size(); i++) {
		std::cout << machInstr[i]; 
		if (i == 6 || i == 11 || i == 16 || i == 19 || i == 24)
			std::cout << "\t";
	}
	std::cout << std::endl;
}

void RV32I::printMach_U()
{
	string b3 = u8Bit(byte3).to_string();
	string b2 = u8Bit(byte2).to_string();
	string b1 = u8Bit(byte1).to_string();
	string b0 = u8Bit(byte0).to_string();
	string machInstr = b3 + b2 + b1 + b0;

	// print out the machine instruction with tabs in between fields
	std::cout << "\t\t\t\timm[31:12]\t\trd\topcode\n";
	std::cout << "Machine instruction:\t\t";
	for (auto i = 0; i < machInstr.size(); i++) {
		std::cout << machInstr[i]; 
		if (i == 19 || i == 24)
			std::cout << "\t";
	}
	std::cout << std::endl;
}

void RV32I::printRegsImm_I(u8 rs1, u8 rd, s16 imm, bool isSigned)
{
	std::cout << "Source register (rs1):\t\tx" << (int) rs1 << " = ";

	if (isSigned)
		std::cout << (int) regs[rs1] << std::endl;
	else
		std::cout << regs[rs1] << std::endl;

	std::cout << "Source reg. bin. (rs1):\t\tx" << (int) rs1 << " = ";
	printBytes(u32Bit(regs[rs1]).to_string());

	std::cout << "Destination register (rd):\tx" << (int) rd << " = ";
	
	if (isSigned)
		std::cout << (int) regs[rd];
	else
		std::cout << regs[rd];
	
	std::cout << " (before instruction)" << std::endl;
	std::cout << "offset (imm):\t\t\t" << (int) imm << std::endl;
}

void RV32I::printRegsImm_S(u8 rs1, u8 rs2, s16 imm)
{
	std::cout << "Source register (rs2):\t\tx" << (int) rs2 << " = ";
	std::cout << regs[rs2] << std::endl;
	std::cout << "Source reg. bin. (rs2):\t\tx" << (int) rs2 << " = ";
	printBytes(u32Bit(regs[rs2]).to_string());
	std::cout << "Source register (rs1):\t\tx" << (int) rs1 << " = ";
	std::cout << regs[rs1] << std::endl;
	std::cout << "Memory offset (imm):\t\t" << (int) imm << std::endl;
	std::cout << "x"
		<< (int) rs1 << " + imm:\t\t\t"
		<< (int) imm + regs[rs1] << std::endl;
}

void RV32I::printRegsImm_R(u8 rs1, u8 rs2, u8 rd, bool isSigned)
{
	std::cout << "Source register (rs2):\t\tx" << (int) rs2 << " = ";

	if (isSigned)
		std::cout << (int) regs[rs2] << std::endl;
	else
		std::cout << regs[rs2] << std::endl;

	std::cout << "Source register (rs1):\t\tx" << (int) rs1 << " = ";

	if (isSigned)
		std::cout << (int) regs[rs1] << std::endl;
	else
		std::cout << regs[rs1] << std::endl;

	std::cout << "Source reg. bin. (rs1):\t\tx" << (int) rs1 << " = ";
	printBytes(u32Bit(regs[rs1]).to_string());
	std::cout << "Destination register (rd):\tx" << (int) rd << " = "
		<< (int) regs[rd] 
		<< " (before instruction)" << std::endl;
}

void RV32I::printRegsImm_U(u8 rd, u32 imm)
{
	std::cout << "Destination register (rd):\tx" << (int) rd << " = "
		<< (int) regs[rd] 
		<< " (before instruction)" << std::endl;
	std::cout << "imm:\t\t\t\t" << (int) imm << std::endl;
}

void RV32I::printRegsImm_B(u8 rs1, u8 rs2, s16 imm, bool isSigned)
{
	if (isSigned) {
		std::cout << "Source register (rs2):\t\tx" << (int) rs2 << " = ";
		std::cout << (int) regs[rs2] << std::endl;
		std::cout << "Source register (rs1):\t\tx" << (int) rs1 << " = ";
		std::cout << (int) regs[rs1] << std::endl;
		std::cout << "imm:\t\t\t\t" << (int) imm << std::endl;
	}
	else {
		std::cout << "Source register (rs2):\t\tx" << (int) rs2 << " = ";
		std::cout << regs[rs2] << std::endl;
		std::cout << "Source register (rs1):\t\tx" << (int) rs1 << " = ";
		std::cout << regs[rs1] << std::endl;
		std::cout << "imm:\t\t\t\t" << (int) imm << std::endl;
	}
}

void RV32I::printMemOffset(u8 rs1, int offset, u8 value)
{
	
	std::cout << "memory[x" << (int) rs1 << " + imm + " << offset << "]:\t\t";
	printBytes(u8Bit(value).to_string());
}

void RV32I::printRdSigned(u8 rd)
{
	std::cout << "rd after instruction:\t\t";
	std::cout << "x" << (int) rd << " = " << (int) regs[rd] << "\n";
	std::cout << "rd after instr. binary:\t\t";
	std::cout << "x" << (int) rd << " = "; 
	printBytes(u32Bit(regs[rd]).to_string());
}
void RV32I::printRdUsigned(u8 rd)
{
	std::cout << "rd after instruction:\t\t";
	std::cout << "x" << (int) rd << " = " << regs[rd] << "\n";
	std::cout << "rd after instr. binary:\t\t";
	std::cout << "x" << (int) rd << " = ";
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

void RV32I::memBoundErr(u8 rs1, s16 imm, int i)
{
	if (regs[rs1] + imm > MaxMemory - i || regs[rs1] + imm < 0) {
		std::cout << "Memory offset (i.e. rs1 + imm.) out of range." << std::endl;
		std::cout << "MaxMemory - i = " << MaxMemory - i << std::endl;
		std::cout << "regs[rs1] + imm = " << regs[rs1] + (int) imm << std::endl;
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

RV32I::u8 RV32I::get31to25()
{
	return byte3 >> 1;
}

RV32I::s16 RV32I::getSplitImm()
{
	// imm[11:5]
	u8 imm11to5 = byte3 >> 1;

	// imm[4:0]
	u8 imm4to0 = (byte0 >> 7) & 1;
	imm4to0   |= ((byte1 << 1) & 0b11110);

	// construct imm[11:0]
	s16 imm  = imm11to5 << 5;
	imm 	|= imm4to0;

	return imm;
}


RV32I::s16 RV32I::getImmed()
{
	s16 imm = (byte2 >> 4) & 0b1111;
	imm |= (byte3 << 4);
	return imm;	
}

RV32I::u32 RV32I::getImm31to12()
{
	u32 imm = byte3 << 12;
	imm    |= byte2 << 4;
	imm    |= byte1 >> 4;
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
