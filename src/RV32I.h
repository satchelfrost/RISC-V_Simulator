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

	typedef signed short s16;
	
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
	void auipc();
	void lui();
	void store();
	void rFormat();	
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

	// immediate instructions
	void addi();
	void slti();
	void sltiu();
	void xori();
	void ori();
	void andi();
	void slli();
	void srli();
	void srai();
	void sr_I();

	// R format instructions
	void add();
	void sub();
	void addSub();
	void sll();
	void slt();
	void sltu();
	void xOr();
	void srl();
	void sra();
	void sr_R();
	void Or();
	void And();

	// Bytes for constructing machine instruction 
	u8 byte0 = 0; // LSB
	u8 byte1 = 0;
	u8 byte2 = 0;
	u8 byte3 = 0; // MSB

	// Print stuff
	void printMach_I();
	void printMach_S();
	void printMach_R();
	void printMach_U();
	void printRegsImm_I(u8 rs1, u8 rd, s16 imm, bool isSigned);
	void printRegsImm_S(u8 rs1, u8 rs2, s16 imm);
	void printRegsImm_R(u8 rs1, u8 rs2, u8 rd, bool isSigned);
	void printRegsImm_U(u8 rd, u32 imm);
	void printMemOffset(u8 rs1, int offset, u8 value);
	void printRdSigned(u8 rd);
	void printRdUsigned(u8 rd);

	// error check (memory in bounds? zero register zero?)
	void zRegError(int value);
	void memBoundErr(u8 rs1, s16 imm, int i);

	// immed, rs1, rd
	u8  getRs1();
	u8  getRs2();
	u8  getRd();
	u8  get31to25();
	s16 getSplitImm();
	s16 getImmed();
	u32 getImm31to12();

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

