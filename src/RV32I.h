#pragma once
#include <string>
#include <bitset>

class RV32I
{
public:
	RV32I();
	RV32I(int test);
	bool Fetch();

private:
	typedef unsigned int   u32;
	typedef unsigned short u16;
	typedef unsigned char   u8;
	typedef std::string string;
	typedef std::bitset<8> u8Bit;
	typedef std::bitset<16> u16Bit;
	typedef std::bitset<32> u32Bit;
	
	bool printInfo = false; 
	bool running = true;
	u32 PC = 0;
	
	static const int MaxMemory = 200;
	u8 memory[MaxMemory];

	// Opcode options
	void load();
	void fence();
	void immediate();
	void jalr();
	void e();
	void aupic();
	void lui();
	void store();
	void regToReg();	
	void branch();
	void jal();
	void error();
	
	void testInstr(int test);

	// Load instructions
	void lb();
	void lh();
	void lw();
	void lbu();
	void lhu();
	void lwu();

	// Store instructions
	void sb();
	void sh();
	void sw();

	// Bytes for constructing machine instruction 
	u8 byte0 = 0; // LSB
	u8 byte1 = 0;
	u8 byte2 = 0;
	u8 byte3 = 0; // MSB

	// Print stuff
	void printMach_I();
	void printMach_S();
	void printRegsImm_I(u8 rs1, u8 rd, u16 imm);
	void printRegsImm_S(u8 rs1, u8 rs2, u16 imm);

	// error check (memory in bounds? zero register zero?)
	void errorCheck_I(u8 rd, u8 rs1, u16 imm);	
	void errorCheck_S(u8 rs1, u8 rs2, u16 imm);	

	// immed, rs1, rd
	u8  getRs1();
	u8  getRs2();
	u8  getRd();
	u8  getSplitImm();
	u16 getImmed();

	// print bytes from binary
	void printBytes(string bin);

	// Registers
	u32 regs[32] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
	};
};

