# RISC-V_Simulator

This project aims to create a RISC-V instruction set simulator (ISS).

### RV32I As described in the specification:

"Although it is convenient to speak of the RISC-V ISA, RISC-V is actually a family of related ISAs, of which there are currently four base ISAs. Each base integer instruction set is characterized by the width of the integer registers and the corresponding size of the address space and by the number of integer registers. There are two primary base integer variants, RV32I and RV64I ..."

Currently I'm only interested in implementing RV32I. Eventually I will look into adding extensions later (such as multiplication, division, floating point numbers etc.) and perhaps the RV64I, but for now I just want to get the project off of the ground.

### Current progress 

Instructions were first manually entered via simulator code (a.k.a. 0's and 1's) directly into memory. This was done to test that each instruction was being interpreted properly by the simulator [sample instructions](https://github.com/satchelfrost/RISC-V_Simulator/blob/master/src/testInstr.h). Registers were also set to arbitrary values if source or destination registers were needed for that particular instruction. Although it looks like I'm biased in using registers x3 and x5, this was an unintentional cosequence of copying and pasting. In reality any of the 32 registers will work as long as the proper binary is in the proper instruction field. See the RISC-V specification under instruction set listings for details on each field. 

In the future I'd like to make an assembler rather than manually entering 0's and 1's!

For debug purposes this interpretation was then logged to [sample ouput](https://github.com/satchelfrost/RISC-V_Simulator/blob/master/sampleOutput) where you can see the output printed. As an example if the instruction in memory is "00000000110100011000001010000011" the sample output in debug-mode will be as follows:

				imm[11:0]	rs1	funct3	rd	opcode
Machine instruction:		000000001101	00011	000	00101	0000011
Assembly:			lb rd, imm(rs1)
Source register (rs1):		x3 = 9
Source reg. bin. (rs1):		x3 = 00000000 00000000 00000000 00001001
Destination register (rd):	x5 = 0 (before instruction)
offset (imm):			13
memory[x3 + imm + 0]:		10000011
rd after instruction:		x5 = -125
rd after instr. binary:		x5 = 11111111 11111111 11111111 10000011

Note that the source register rs1 is "00011" in this case. The interpreter knows this and accordingly prints "x3 = ...", any register will work though. Also note that the message may vary slightly depending on the instruction. When not in debug-mode the instruction will simply be executed without any printing / logging. 
