#include <iomanip>

#include "../include/simulator.hpp"
#include "../include/defs.hpp"

using namespace std;

Simulator::Simulator() {
	pc = ADDR_INSTR;
}

void Simulator::run_inst() {
	opcode = curr_inst >> 26;
	if (opcode == 0) { // r-type
		r_rs = (curr_inst >> 21) & MASK_5_BIT;
		r_rt = (curr_inst >> 16) & MASK_5_BIT;
		r_rd = (curr_inst >> 11) & MASK_5_BIT;
		r_sa = (curr_inst >> 6) & MASK_5_BIT;
		r_fncode = (curr_inst) & MASK_6_BIT;
	
		call_r(r_fncode);
	}
	else if (opcode == 2 || opcode == 3) { // j-type
		j_i = curr_inst & MASK_26_BIT;

		call_j(opcode);
	}
	else { // i-type
		i_rs = (curr_inst >> 21) & MASK_5_BIT;
		i_rd = (curr_inst >> 16) & MASK_5_BIT;
		i_i = (curr_inst) & MASK_16_BIT;
			
		call_i(opcode);
	}
}

int Simulator::init(const string& in_filename) {
	// open file in binary mode
	filename = in_filename;
	
	ifstream bin_file(filename.c_str(), ios::binary);
	
	if (bin_file.is_open()) {
		uint8_t buffer; // buffer for current inst
		while(bin_file.read((char*) &buffer, sizeof(buffer))) {
			memory.write_instr(buffer);
			log8(buffer);	
		}
		
		debug << "Successfully read." << endl;
		int returnVal = run();
		
		if (LOG_DEBUG) getlogs();
		debug << endl << "===== PROGRAM COMPLETED =====" << endl;
		
		return returnVal;
	}
	
	else {
		// catch for file not found
		cerr << "I/O Error (-21): File not found!" << endl;
		if (LOG_DEBUG) getlogs();
		exit(IO_ERROR);
	}
}

int Simulator::run() {
	while (pc != ADDR_NULL) { // if pc == ADDR_NULL, means done with program
		debug << "============================" << endl;
		debug << "Current PC   : 0x" << setfill('0') << setw(8) << hex << uppercase << pc << endl;
		curr_inst = memory.read_instr(pc);
		debug << "Current Instr: 0x" << setfill('0') << setw(8) << hex << uppercase << curr_inst << endl;
		run_inst();
		
		// update pc, based on delayed branch
		if (branch) {
			pc = branch_addr;
			branch = false;
		}
		else if (call_branch) {
			pc += 4;
			call_branch = false;
			branch = true;
		}
		else {
			pc += 4;
		}
	}
	
	return reg[2] & MASK_8_BIT;
}

// write registers from $1 - $31
void Simulator::write_reg(const int& idx, const uint32_t& val) {
	if (idx == 0) {
		debug << "Cannot write to $zero, write ignored." << endl;
		return;
	}
	reg[idx] = val;
	return;
}

// debug : dump registers and memory
void Simulator::getlogs() {
	ofstream outfile;
	string outfilename = filename + ".log";
	outfile.open(outfilename, ios::out);
	
	if (outfile.is_open()) {
		outfile << "===== REGISTERS =====" << endl;
		outfile << setw(5) << "REG";
		outfile << setw(12) << "HEX";
		outfile << setw(16) << "UNSIGNED";
		outfile << setw(16) << "SIGNED";
		outfile << setw(40) << "BINARY" << endl;
		for (int i = 0; i < 32; i++) {
			unsigned us = reg[i];
			int is = reg[i];
			bitset<32> bi(reg[i]);
			string regstr = "$" + to_string(i);
			outfile << setw(5) << regstr;
			outfile << "  ";
			outfile << "0x" << setfill('0') << setw(8) << hex << uppercase << reg[i] << setfill(' ');
			outfile << setw(16) << dec << us;
			outfile << setw(16) << dec << is;
			outfile << setw(40) << bi;
			outfile << endl;
		}
		outfile << endl;
		outfile << endl;
		
		outfile << "===== NON-ZERO MEMORY =====" << endl;
		outfile << setw(12) << "ADDR" << setw(12) << "DATA" << endl;
		for (uint32_t i = 0x20000000; i < 0x24000000; i += 4) {
			uint32_t tmp = memory.read_word(i);
			if (tmp) {
				outfile << "  0x" << setfill('0') << setw(8) << hex << uppercase << i << setfill(' ');
				outfile << "  0x" << setfill('0') << setw(8) << hex << uppercase << tmp << setfill(' ');
				outfile << endl;
			}
		}
		outfile << endl;
		
		debug << endl << "===== Register & Memory dump: " << outfilename << " =====" << endl;
		outfile.close();
	}
	else {
		debug << "Can't open outfile file!" << endl;
		exit(EXIT_FAILURE);
	}
	return;
}

