#include "../../include/defs.hpp"
#include "../include/simulator.hpp"

using namespace std;

void Simulator::call_r(uint8_t param) {
	if (LOG_DEBUG) {
		debugsrc1 = r_rs;
		debugsrc2 = r_rt;
	}
	switch (param) {
		case 32:
			add();
			break;
		case 33:
			addu();
			break;
		case 36:
			bitwise_and();
			break;
		case 26:
			div();
			break;
		case 27:
			divu();
			break;
		case 8:
			jr();
			break;
		case 9:
			jalr();
			break;
		case 16:
			mfhi();
			break;
		case 17:
			mthi();
			break;
		case 18:
			mflo();
			break;
		case 19:
			mtlo();
			break;
		case 24:
			mult();
			break;
		case 25:
			multu();
			break;
		case 37:
			bitwise_or();
			break;
		case 0:
			sll();
			break;
		case 4:
			sllv();
			break;
		case 42:
			slt();
			break;
		case 43:
			sltu();
			break;
		case 3:
			sra();
			break;
		case 7:
			srav();
			break;
		case 2:
			srl();
			break;
		case 6:
			srlv();
			break;
		case 34:
			sub();
			break;
		case 35:
			subu();
			break;
		case 38:
			bitwise_xor();
			break;
		default:
            handleError(INVALID_INSTRUCTION, "R type function not available.");
			break;
	}
	return;
}
void Simulator::call_i(uint8_t param) {
	if (LOG_DEBUG) {
		debugsrc = i_rs;
	}
	switch (param) {
		case 8:
			addi();
			break;
		case 9:
			addiu();
			break;
		case 12:
			andi();
			break;
		case 4:
			beq();
			break;
		case 1:
			call_cond_B();
			break;
		case 7:
			bgtz();
			break;
		case 6:
			blez();
			break;
		case 5:
			bne();
			break;
		case 32:
			lb();
			break;
		case 36:
			lbu();
			break;
		case 33:
			lh();
			break;
		case 37:
			lhu();
			break;
		case 15:
			lui();
			break;
		case 35:
			lw();
			break;
		case 34:
			lwl();
			break;
		case 38:
			lwr();
			break;
		case 13:
			ori();
			break;
		case 40:
			sb();
			break;
		case 41:
			sh();
			break;
		case 10:
			slti();
			break;
		case 11:
			sltiu();
			break;
		case 43:
			sw();
			break;
		case 14:
			xori();
			break;
		default:
			cerr << "I type function not available." << endl;
			// if (LOG_DEBUG) getlogs();
			exit(INVALID_INSTRUCTION);	
			break;
	}
	return;
}

void Simulator::call_cond_B() {
	switch(i_rd) {
		case 0:
			bltz();
			break;
		case 1:
			bgez();
			break;
		case 16:
			bltzal();
			break;
		case 17:
			bgezal();
			break;
		default:
			cerr << "I type function not available." << endl;
			// if (LOG_DEBUG) getlogs();
			exit(INVALID_INSTRUCTION);	
			break;
	}

}

void Simulator::call_j(uint8_t param) {
	switch (param) {
		case 2:
			j();
			break;
		case 3:
			jal();
			break;
		default:
			cerr << "J type function not available." << endl;
			// if (LOG_DEBUG) getlogs();
			exit(INVALID_INSTRUCTION);	
			break;
	}
	return;
}
