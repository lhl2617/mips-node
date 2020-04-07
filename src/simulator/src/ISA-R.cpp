#include "../include/simulator.hpp"
#include "../include/defs.hpp"

using namespace std;

void Simulator::add() {
	debug << "Add (with overflow)" << endl;
	
	int32_t result, src1, src2;
	src1 = reg[r_rs];
	src2 = reg[r_rt];
	
	result = src1 + src2;
	
	// overflow catch
	if ((src1 >= 0 && src2 >= 0 && result < 0) ||
		(src1 < 0 && src2 < 0 && result >= 0)) {
			debug << "!!!!! Overflow !!!!!" << endl;
			debug << "add: " << "$" << +r_rd << " = $" << +r_rs << " + $" << +r_rt << endl;
			debug << dec << result << " = " << src1 << " + " << src2 << endl;
			if (LOG_DEBUG) getlogs();
			exit(ARITHMETIC_EXCEPTION);
	}
	
	write_reg(r_rd, result);
	
	debug << "add: " << "$" << +r_rd << " = $" << +r_rs << " + $" << +r_rt << endl;
	debug << dec << "DEC " << (int32_t)reg[r_rd] << " = " << src1 << " + " << src2 << endl;
	return;
}

void Simulator::addu() {
	debug << "Add unsigned (no overflow)" << endl;
	
	uint32_t result, src1, src2;
	src1 = reg[r_rs];
	src2 = reg[r_rt];
	
	result = src1 + src2;
	
	write_reg(r_rd, result);
	
	debug << "addu: " << "$" << +r_rd << " = $" << +r_rs << " + $" << +r_rt << endl;
	debug << dec << "DEC " << reg[r_rd] << " = " << src1 << " + " << src2 << endl;
	return;
}

void Simulator::bitwise_and() {
	debug << "Bitwise and" << endl;
	
	uint32_t result;
	
	result = reg[r_rs] & reg[r_rt];
	
	write_reg(r_rd, result);
	
	debug << "and: " << "$" << +r_rd << " = $" << +r_rs << " & $" << +r_rt << endl;
	debug << hex << "HEX " << reg[r_rd] << " = " << debugsrc1 << " & " << debugsrc2 << endl;
}

void Simulator::div() {
	debug << "Divide" << endl;
	
	if (reg[r_rt] == 0) {
		debug << "!!!!! Division by Zero !!!!! (no exception, no effect)" << endl;
		return;
	}
	
	// treat as signed 32-bit integers
	int32_t src1 = reg[r_rs];
	int32_t src2 = reg[r_rt];
	
	regHI = uint32_t(src1 % src2);
	regLO = uint32_t(src1 / src2);
	
	debug << "div: " << "$HI = $" << +r_rs << " % $" << +r_rt << endl;
	debug << "div: " << "$LO = $" << +r_rs << " / $" << +r_rt << endl;
	debug << dec << "DEC " << regHI << " = " << src1 << " % " << src2 << endl;
	debug << dec << "DEC " << regLO << " = " << src1 << " / " << src2 << endl;

	return;
}

void Simulator::divu() {
	debug << "Divide unsigned" << endl;
	
	if (reg[r_rt] == 0) {
		debug << "!!!!! Division by Zero !!!!! (no exception, no effect)" << endl;
		return;
	}
	
	// treat as unsigned 32-bit integers
	uint32_t src1 = reg[r_rs];
	uint32_t src2 = reg[r_rt];
	
	regHI = src1 % src2;
	regLO = src1 / src2;
	
	debug << "divu: " << "$HI = $" << +r_rs << " % $" << +r_rt << endl;
	debug << "divu: " << "$LO = $" << +r_rs << " / $" << +r_rt << endl;
	debug << dec << "DEC " << regHI << " = " << src1 << " % " << src2 << endl;
	debug << dec << "DEC " << regLO << " = " << src1 << " / " << src2 << endl;

	return;
}

void Simulator::jr() {
	debug << "Jump Register" << endl;
	
	branch_addr = reg[r_rs];
	call_branch = true;
	
	debug << "jr: " << "PC = $" << +r_rs << endl;
	debug << hex << "HEX " << "PC = " << branch_addr << endl;
	
	return;
}

void Simulator::jalr() {
	debug << "Jump and Link Register" << endl;
	
	branch_addr = reg[r_rs];
	call_branch = true;
	write_reg(r_rd, pc + 8);
	
	debug << "jalr: " << "PC = $" << +r_rs << "; return_addr in $" << +r_rd << endl;
	debug << hex << "HEX " << "PC = " << branch_addr << "; " << "return addr: " << +r_rd << " return pc: " << reg[r_rd] << endl;
	
	return;
}

void Simulator::mfhi() {
	debug << "Move from HI" << endl;
	
	int32_t result;
	
	result = regHI;
	
	write_reg(r_rd, result);
	
	debug << "mfhi: $" << +r_rd << " = $hi"  << endl;
	debug << hex << "HEX " << (int32_t) reg[r_rd] << " = " << regHI << endl;
	
	return;
}

void Simulator::mflo() {
	debug << "Move from LO" << endl;
	
	int32_t result;
	
	result = regLO;
	
	write_reg(r_rd, result);
	
	debug << "mflo: $" << +r_rd << " = $lo" << endl;
	debug << hex << "HEX " << (int32_t) reg[r_rd] << " = " << regLO << endl;
	
	return;
}

void Simulator::mthi() {
	debug << "Move to HI" << endl;
	
	int32_t result = reg[r_rs];
	
	regHI = result;
	
	debug << "mthi: $hi = $" << +r_rs << endl;
	debug << hex << "HEX " << (int32_t) regHI << " = " << result << endl;
	
	return;
}

void Simulator::mtlo() {
	debug << "Move to LO" << endl;
	
	int32_t result = reg[r_rs];
	
	regLO = result;
	
	debug << "mtlo: $lo = $" << +r_rs << endl;
	debug << hex << "HEX " << (int32_t) regLO << " = " << result << endl;
	
	return;
}

void Simulator::mult() {
	debug << "Multiply" << endl;
	
	int64_t full_result = (int64_t)(int32_t) reg[r_rs] * (int64_t)(int32_t) reg[r_rt];
	regHI = (uint32_t) (full_result >> 32);
	regLO = (uint32_t) (full_result & 0xFFFFFFFF);
	
	debug << "mult: " << "$HI = $" << +r_rs << " * $" << +r_rt << " overflow " << endl;
	debug << "mult: " << "$LO = $" << +r_rs << " * $" << +r_rt << " non-overflow" << endl;
	debug << dec << "DEC " << regHI << " = " << debugsrc1 << " * " << debugsrc2 << " >> 32"<< endl;
	debug << dec << "DEC " << regLO << " = " << debugsrc1 << " * " << debugsrc2 << " & 0xFFFFFFFF" << endl;
	
	return;
}

void Simulator::multu() {
	debug << "Multiply Unsigned" << endl;
	
	uint64_t full_result = (uint64_t) reg[r_rs] * (uint64_t) reg[r_rt];
	regHI = (uint32_t) (full_result >> 32);
	regLO = (uint32_t) (full_result & 0xFFFFFFFF);
	
	debug << "multu: " << "$HI = $" << +r_rs << " * $" << +r_rt << " overflow " << endl;
	debug << "multu: " << "$LO = $" << +r_rs << " * $" << +r_rt << " non-overflow" << endl;
	debug << dec << "DEC " << regHI << " = " << debugsrc1 << " * " << debugsrc2 << " >> 32"<< endl;
	debug << dec << "DEC " << regLO << " = " << debugsrc1 << " * " << debugsrc2 << " & 0xFFFFFFFF" << endl;
	
	return;
}

void Simulator::bitwise_or() {
	debug << "Bitwise OR" << endl;
	
	uint32_t result;
	
	result = reg[r_rs] | reg[r_rt];
	
	write_reg(r_rd, result);
	
	debug << "or: " << "$" << +r_rd << " = $" << +r_rs << " | $" << +r_rt << endl;
	debug << hex << "HEX " << reg[r_rd] << " = " << debugsrc1 << " | " << debugsrc2 << endl;
	
	return;
}

void Simulator::sll() {
	debug << "Shift left logical" << endl;
	
	uint32_t result;
	
	result = reg[r_rt] << +r_sa;
	
	write_reg(r_rd, result);
	
	debug << "sll: " << "$" << +r_rd << " = $" << +r_rt << " << " << +r_sa << endl;
	debug << hex << "HEX " << reg[r_rd] << " = " << debugsrc2 << " << " << +r_sa << endl;
	return;
}

void Simulator::sllv() {
	debug << "Shift left logical variable" << endl;
	
	uint32_t result;
	
	result = reg[r_rt] << (reg[r_rs] & MASK_5_BIT);
	
	write_reg(r_rd, result);
	
	debug << "sll: " << "$" << +r_rd << " = $" << +r_rt << " << " << "& 0b1111" << endl;
	debug << hex << "HEX " << reg[r_rd] << " = " << debugsrc2 << " << " << debugsrc1 << " & 0b1111" << endl;
	
	return;
}

void Simulator::slt() {
	debug << "Set on less than (signed)" << endl;
	
	uint32_t result;
	
	// treat as signed
	int32_t src1 = reg[r_rs];
	int32_t src2 = reg[r_rt];
	
	if (src1 < src2) {
		result = 1;
	}
	else {
		result = 0;
	}
	
	write_reg(r_rd, result);
	
	debug << "slt: $" << +r_rd << " = $" << +r_rs << " < $" << +r_rt << endl;
	debug << dec << "DEC " << reg[r_rd] << " = " << src1 << " < " << src2 << endl;
	return;
}

void Simulator::sltu() {
	debug << "Set on less than (unsigned)" << endl;
	
	uint32_t result;
	
	if (reg[r_rs] < reg[r_rt]) {
		result = 1;
	}
	else {
		result = 0;
	}
	
	write_reg(r_rd, result);
	
	debug << "sltu: $" << +r_rd << " = $" << +r_rs << " < $" << +r_rt << endl;
	debug << dec << "DEC " << reg[r_rd] << " = " << debugsrc1 << " < " << debugsrc2 << endl;
	return;
}
		
void Simulator::sra() {
	debug << "Shift right arithmetic" << endl;
	
	uint32_t result;
	
	result = (int32_t) reg[r_rt] >> r_sa;
	
	write_reg(r_rd, result);
	
	debug << "sra: $" << +r_rd << " = $" << +r_rt << " >> " << +r_sa << endl;
	debug << hex << "HEX " << reg[r_rd] << " = " << debugsrc2 << " >> " << +r_sa << endl;		
	
	return;
}

void Simulator::srav() {
	debug << "Shift right arithmetic variable" << endl;
	
	uint32_t result;
	
	result = (int32_t) reg[r_rt] >> (reg[r_rs] & MASK_5_BIT);
	
	write_reg(r_rd, result);
	
	debug << "srav: " << "$" << +r_rd << " = $" << +r_rt << " >> " << "& 0b1111" << endl;
	debug << hex << "HEX " << reg[r_rd] << " = " << debugsrc2 << " >> " << debugsrc1 << " & 0b1111" << endl;
	return;
}

void Simulator::srl() {
	debug << "Shift right logical" << endl;
	
	uint32_t result;
	
	result = reg[r_rt] >> r_sa;
	
	write_reg(r_rd, result);
	
	debug << "sll: " << "$" << +r_rd << " = $" << +r_rt << " >> " << +r_sa << endl;
	debug << hex << "HEX " << reg[r_rd] << " = " << debugsrc2 << " >> " << +r_sa << endl;
	
	return;
}

void Simulator::srlv() {
	debug << "Shift right logical variable" << endl;
	
	uint32_t result;
	
	result = reg[r_rt] >> (reg[r_rs] & MASK_5_BIT);
	
	write_reg(r_rd, result);
	
	debug << "sll: " << "$" << +r_rd << " = $" << +r_rt << " >> " << "& 0b1111" << endl;
	debug << hex << "HEX "<< reg[r_rd] << " = " << debugsrc2 << " >> " << debugsrc1 << " & 0b1111" << endl;
	
	return;
}

void Simulator::sub() {
	debug << "Subtract (with overflow)" << endl;
	
	int32_t result, src1, src2;
	src1 = reg[r_rs];
	src2 = reg[r_rt];
	
	result = src1 - src2;

	// overflow catch
	if ((src1 >= 0 && src2 < 0 && result < 0) ||
	 (src1 < 0 && src2 >= 0 && result >= 0)) {
		debug << "!!!!! Overflow !!!!!" << endl; 
		debug << "sub: " << "$" << +r_rd << " = $" << +r_rs << " - $" << +r_rt << endl;
		debug << dec << "DEC " << (int32_t) reg[r_rd] << " = " << src1 << " - " << src2 << endl;
		if (LOG_DEBUG) getlogs();
		exit(ARITHMETIC_EXCEPTION);
}

	write_reg(r_rd, result);
	
	debug << "sub: " << "$" << +r_rd << " = $" << +r_rs << " - $" << +r_rt << endl;
	debug << dec << "DEC " << (int32_t) reg[r_rd] << " = " << src1 << " - " << src2 << endl;
	return;
}

void Simulator::subu() {
	debug << "Subtract unsigned" << endl;
	
	uint32_t result, src1, src2;
	src1 = reg[r_rs];
	src2 = reg[r_rt];
	
	result = src1 - src2;
	
	write_reg(r_rd, result);
	
	debug << "subu: " << "$" << +r_rd << " = $" << +r_rs << " - $" << +r_rt << endl;
	debug << dec << "DEC " << (int32_t) reg[r_rd] << " = " << src1 << " - " << src2 << endl;
	return;
}


void Simulator::bitwise_xor() {
	debug << "Bitwise XOR" << endl;
	
	uint32_t result;
	
	result = reg[r_rs] ^ reg[r_rt];
	
	write_reg(r_rd, result);
	
	debug << "xor: " << "$" << +r_rd << " = $" << +r_rs << " ^ $" << +r_rt << endl;
	debug << hex << "HEX " << reg[r_rd] << " = " << debugsrc1 << " ^ " << debugsrc2 << endl;
	
	return;
}
