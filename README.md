# RISC-V_Simulator

This project aims to create an instruction set simulator (ISS) for the RV32I instruction set.

### Sample Instructions loaded from memory:

imm[11:0]	rs1	funct3	rd	opcode
Machine instruction:		000000001101	00011	000	00101	0000011
Assembly:			lb rd, imm(rs1)
Source register (rs1):		x3 = 9
Destination register (rd):	x5 = 0 (before instruction)
Memory offset (imm):		13
x3 + imm:			22
memory[x3 + imm]:		-125 (bin: 10000011)
rd after instruction:		x5 = -125 (bin: 11111111 11111111 11111111 10000011)


				imm[11:0]	rs1	funct3	rd	opcode
Machine instruction:		000000001101	00011	001	00101	0000011
Assembly:			lh rd, imm(rs1)
Source register (rs1):		x3 = 9
Destination register (rd):	x5 = 0 (before instruction)
Memory offset (imm):		13
x3 + imm:			22
memory[x3 + imm + 1]:		-124 (bin: 10000100)
memory[x3 + imm + 0]:		42 (bin: 00101010)
Half word:			-31702 (bin: 10000100 00101010)
rd after instruction:		x5 = -31702 (bin: 11111111 11111111 10000100 00101010)


				imm[11:0]	rs1	funct3	rd	opcode
Machine instruction:		000000001101	00011	010	00101	0000011
Assembly:			lw rd, imm(rs1)
Source register (rs1):		x3 = 9
Destination register (rd):	x5 = 0 (before instruction)
Memory offset (imm):		13
x3 + imm:			22
memory[x3 + imm + 3]:		-123 (bin: 10000101)
memory[x3 + imm + 2]:		44 (bin: 00101100)
memory[x3 + imm + 1]:		43 (bin: 00101011)
memory[x3 + imm + 0]:		42 (bin: 00101010)
Word (signed):			-2060702934 (bin: 10000101 00101100 00101011 00101010)
rd after instruction:		x5 = -2060702934 (bin: 10000101 00101100 00101011 00101010)
