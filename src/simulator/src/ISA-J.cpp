#include "../include/simulator.hpp"
#include "../../include/defs.hpp"

using namespace std;

void Simulator::j() {
    debug << "Jump" << endl;

    uint32_t offset = (j_i << 2);
    branch_addr = (offset | ((pc + 4) & 0xF0000000));
    call_branch = true;
    
	debug << hex << "HEX " << "j: branchPC = " << branch_addr << " j_i = " << j_i << endl;
	
    return;
}


void Simulator::jal() {
    debug << "Jump and Link" << endl;
	
	reg[31] = pc + 8;
    uint32_t offset = (j_i << 2);
    branch_addr = (offset | ((pc + 4) & 0xF0000000));
    call_branch = true;
    
	debug << "jal: branchPC = " << branch_addr << " j_i = " << j_i << endl;
	debug << hex << "HEX " << "reg[31] = " << pc + 8 << endl;
    
    return;
}
