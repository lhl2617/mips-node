#include "../include/simulator.hpp"
#include "../../include/defs.hpp"

using namespace std;

//sign extend from 16 to 32 bits
int32_t Simulator::sign_extend_16(int16_t num) {
	int32_t r;
	struct {signed int num:16;} s;
	r = s.num = num;
	return r;
}

int32_t Simulator::sign_extend_8(int8_t num) {
    int32_t r;
    struct {signed int num:8;} s;
    r = s.num = num;
    return r;
}

void Simulator::addi()
{
    debug << "Add Immediate word (with overflow)" << endl;
    
    int32_t result, src, immediate;
	src = reg[i_rs];
	immediate = sign_extend_16(i_i);
    
    result = src + immediate;
    
    // overflow catch
    if ((src >= 0 && immediate >= 0 && result < 0) ||
        (src < 0 && immediate < 0 && result >= 0)) {
		debug << "!!!!! Overflow !!!!!" << endl;
		debug << "addi: " << "$" << +i_rd << " = $" << +i_rs << " + " << immediate << endl;
		debug << dec << "DEC " << (int32_t) reg[i_rd] << " = " << src << " + " << immediate << endl;
		// if (LOG_DEBUG) getlogs();
        exit(ARITHMETIC_EXCEPTION);
    }
    
    write_reg(i_rd, result);
    
    debug << "addi: " << "$" << +i_rd << " = $" << +i_rs << " + " << immediate << endl;
    debug << dec << "DEC " << (int32_t) reg[i_rd] << " = " << src << " + " << immediate << endl;
    
    return;
}

void Simulator::addiu()
{
    debug << "Add immediate unsigned (no overflow)" << endl;
    
    int32_t result, src, immediate;
	src = reg[i_rs];
	immediate = sign_extend_16(i_i);
    
    result = src + immediate;
    
    write_reg(i_rd, result);
    
    debug << "addiu: " << "$" << +i_rd << " = $" << +i_rs << " + " << immediate << endl;
    debug << dec << "DEC " << (int32_t) reg[i_rd] << " = " << src << " + " << immediate << endl;
   
    return;
}

void Simulator::andi()
{
    debug << "And Immediate" << endl;
	
    uint32_t result;
    
    result = reg[i_rs] & i_i;
    
    write_reg(i_rd, result);
    
    debug << "andi: " << "$" << +i_rd << " = $" << +i_rs << " & " << +i_i << endl;
    debug << hex << "HEX " << reg[i_rd] << " = " << debugsrc << " & " << i_i << endl;
	
    return;
}


void Simulator::beq()
{
    debug << "Branch on Equal" << endl;

    if (reg[i_rs] == reg[i_rd])
    {
        int32_t offset = (sign_extend_16(i_i) << 2);
        branch_addr = pc + offset + 4;
        call_branch = true;
        
        debug << "beq: branch taken, pc: " << pc << ". offset was " << offset << endl;
		debug << dec << "DEC " << reg[i_rs] << " == " << reg[i_rd] << endl;
    }
    
    else {
        debug << "beq: branch not taken" << endl;
        debug << dec << "DEC " << reg[i_rs] << " != " << reg[i_rd] << endl;
    }
    
    return;
}

void Simulator::bgez()
{
    debug << "Branch on Greater Than or Equal to Zero" << endl;
                
    if((int32_t)reg[i_rs] >= 0)
    {
        int32_t offset = (sign_extend_16(i_i) << 2);
        branch_addr = pc + offset + 4;
        call_branch = true;
        
        debug << "bgez: branch taken, pc is now " << pc << ". offset was " << offset << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  >= 0" << endl;
    }
    
    else{
        debug << "bgez: branch not taken" << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << " !>= 0" << endl;
    }
                            
    return;
}
				  
//using the reg 31, an register used for branching with link - to store return address
//recall that pc here is incremented by 4, no need to multiple by 4
				  
void Simulator::bgezal()
{
    debug << "Branch on Greater Than or Equal to Zero and Link" << endl;

    reg[31] = pc + 8;
    debug << "reg[31] = " << reg[31] << endl;
    if((int32_t) reg[i_rs] >= 0)
    {
        int32_t offset = (sign_extend_16(i_i) << 2);
        branch_addr = pc + offset + 4;
        call_branch = true;
        
        debug << "bgezal: branch taken, pc is now " << pc << ". offset was " << offset << endl;
        debug << "bgezal: return address reg[31] is " << reg[31];
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  >= 0" << endl;
    }
    
    else{
        debug << "bgezal: branch not taken" << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  !>= 0" << endl;
    }

    return;                  
}


void Simulator::bgtz() 
{     
    debug << "Branch on Greater Than Zero" << endl;
    
    if((int32_t)reg[i_rs] > 0)
    {
        int32_t offset = (sign_extend_16(i_i) << 2);
        branch_addr = pc + offset + 4;
        call_branch = true;
        
        debug << "bgtz: branch taken, pc is now " << pc << ". offset was " << offset << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  > 0" << endl;
    }
    
    else{
        debug << "bgtz: branch not taken" << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  !> 0" << endl;
    }

    return; 
}


void Simulator::blez() 
{ 
    debug << "Branch on Less Than or Equal to Zero" << endl;
    
    if((int32_t)reg[i_rs] <= 0)
    {
        int32_t offset = (sign_extend_16(i_i) << 2);
        branch_addr = pc + offset + 4;
        call_branch = true;
        
        debug << "blez: branch taken, pc is now " << pc << ". offset was " << offset << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  <= 0" << endl;
    }
    
    else{
        debug << "blez: branch not taken" << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  !<= 0" << endl;
    }
    
    return; 
}


void Simulator::bltz()
{ 
    debug << "Branch on Less Than Zero" << endl;
    
    if((int32_t)reg[i_rs] < 0)
    {
        int32_t offset = (sign_extend_16(i_i) << 2);
        branch_addr = pc + offset + 4;
        call_branch = true;
        
        debug << "bltz: branch taken, pc is now " << pc << ". offset was " << offset << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  < 0" << endl;
    }
    
    else{
        debug << "bltz: branch not taken" << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  !< 0" << endl;
    }
    
    return; 
}


void Simulator::bltzal()
{ 
    debug << "Branch on Less Than Zero and Link" << endl;
    

    reg[31] = pc + 8;

    if((int32_t)reg[i_rs] < 0)
    {
        
        int32_t offset = (sign_extend_16(i_i) << 2);
        branch_addr = pc + offset + 4;
        call_branch = true;
        
        debug << "bltzal: branch taken, pc is now " << pc << ". offset was " << offset << endl;
        debug << "bltzal: return address reg[31] is " << reg[31];
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  < 0" << endl;
    }
    
    else{
        debug << "bltzal: branch not taken" << endl;
		debug << dec << "DEC " << (int32_t)reg[i_rs] << "  !< 0" << endl;
    }
    
    return; 
}


void Simulator::bne() 
{
    debug << "Branch on Not Equal" << endl;
    
    if(reg[i_rs] != reg[i_rd])
    {
        int32_t offset = (sign_extend_16(i_i) << 2);
        branch_addr = pc + offset + 4;
        call_branch = true;
        
        debug << "bne: branch taken, pc is now " << pc << ". offset was " << offset << endl;
		debug << dec << "DEC " << reg[i_rs] << " != " << reg[i_rd] << endl;
    }
    
    else{
        debug << "bne: branch not taken" << endl;
		debug << dec << "DEC " << reg[i_rs] << " !!= " << reg[i_rd] << endl;
    }
    
    return;
}


void Simulator::lb() 
{
    debug << "Load Byte" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];

    int32_t result= sign_extend_8(memory.read_byte(effAddr));
	
    write_reg(i_rd, result);

    debug << "lb: $" << +i_rd << " = " << "memory[$" << +i_rs <<" + "<< +i_i << "]" << endl;
	debug << hex << "HEX " << reg[i_rd] << " = " << reg[i_rs] << " + " << +i_i << endl;
    
    return;
}


void Simulator::lbu()
{
    debug << "Load Byte Unsigned" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];
    
    int32_t result = memory.read_byte(effAddr);
    result = (result & MASK_8_BIT);
    write_reg(i_rd, result);
    
    debug << "lbu: $" << +i_rd << " = " << "memory[$" << +i_rs <<" + "<< +i_i << "]" << endl;
    debug << hex << "HEX " << reg[i_rd] << " = " << reg[i_rs] << " + " << +i_i << endl;
    
    return;
}


void Simulator::lh()
{
    debug << "Load Halfword" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];
    
    int32_t result = sign_extend_16(memory.read_half_word(effAddr));
	
    write_reg(i_rd, result);
    
    debug << "lh: $" << +i_rd << " = " << "memory[$" << +i_rs <<" + "<< +i_i << "]" <<  endl;
    debug << hex << "HEX " << reg[i_rd] << " = " << reg[i_rs] << " + " << +i_i << endl;
    
    return;
}


void Simulator::lhu()
{
    debug << "Load Halfword Unsigned" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];

    int32_t result = memory.read_half_word(effAddr);
	result = (result & MASK_16_BIT);
    write_reg(i_rd, result);
    
    debug << "lhu: $" << +i_rd << " = " << "memory[$" << +i_rs <<" + "<< +i_i << "]" <<  endl;
    debug << hex << "HEX " << reg[i_rd] << " = " << reg[i_rs] << " + " << +i_i << endl;
    
    return;
}


void Simulator::lui()
{
    debug << "Load Upper Immediate" << endl;

    uint32_t result = i_i << 16;
    write_reg(i_rd, result);
    
    debug << "lui: $" << +i_rd << " = " << reg[i_rd] <<", i_i = " << +i_i << "<< 16" << endl;
    debug << hex << "HEX " << reg[i_rd] << " = "<< +i_i << " << 16" << endl;
        
    return;
}


void Simulator::lw()
{
    debug << "Load Word" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];
    
    int32_t result = memory.read_word(effAddr);
    write_reg(i_rd, result);
    
    debug << "lw: $" << +i_rd << " = " << "memory[$" << +i_rs <<" + "<< +i_i << "]" <<  endl;
    debug << hex << "HEX " << reg[i_rd] << " = " << reg[i_rs] << " + " << +i_i << endl;
    
    return;
}


void Simulator::lwl()
{
    debug << "Load Word Left" << endl;

    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];
    uint32_t ori_rt_data = reg[i_rd];
    
    // mode = offset(vAddr [1...0]
    uint32_t mode = effAddr & 3;
    uint32_t aligned_addr = effAddr - mode;
    
    uint32_t aligned_word = memory.read_word(aligned_addr);
    uint32_t result;
    
    //start cutting and pasting
    uint32_t mask_aligned_word = 0xffffffff;
    uint32_t mask_ori_rt_data = 0xffffffff;
    
    mask_aligned_word >>= (mode << 3);
    mask_ori_rt_data >>= ((4 - mode) << 3);
    
    if (!mode) mask_ori_rt_data = 0;

    aligned_word = (aligned_word & mask_aligned_word) << (mode << 3);
    ori_rt_data = (ori_rt_data & mask_ori_rt_data);

    result = ori_rt_data + aligned_word;
    write_reg(i_rd, result);
    
    debug << "lwl: mode = " << mode << ", result = " << result << endl;
    debug << hex << "HEX $" << +i_rd << " = " << result << endl;

    return;
}


void Simulator::lwr()
{
    debug << "Load Word Right" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];
    uint32_t ori_rt_data = reg[i_rd];
    
    // mode = offset(vAddr [1...0]
    uint32_t mode = effAddr & 3;
    uint32_t aligned_addr = effAddr - mode;
    
    uint32_t aligned_word = memory.read_word(aligned_addr);
    uint32_t result;
    
    //start cutting and pasting
    uint32_t mask_aligned_word = 0xffffffff;
    uint32_t mask_ori_rt_data = 0xffffffff;
    
    mask_aligned_word <<= ((3 - mode) << 3);
    mask_ori_rt_data <<= ((mode + 1) << 3);
    
    if (mode == 3) mask_ori_rt_data = 0;

    aligned_word = (aligned_word & mask_aligned_word) >> ((3 - mode) << 3);
    ori_rt_data = (ori_rt_data & mask_ori_rt_data);

    result = ori_rt_data + aligned_word;
    write_reg(i_rd, result);
    
    
    debug << "lwr: mode = " << mode << ", result = " << result << endl;
    debug << hex << "HEX $" << +i_rd << " = " << result << endl;
    
    return;
}


void Simulator::ori()
{
    debug << "Or Immediate" << endl;
    int32_t result = reg[i_rs] | (i_i);
    write_reg(i_rd, result);
     
    debug << "ori: " << "$" << +i_rd << " = $" << +i_rs << " ^ " << +i_i << endl;
    debug << hex << "HEX " << reg[i_rd] << " = " << +debugsrc << " | " << +i_i << endl;
     
    return;
}


void Simulator::sb()
{
    debug << "Store Byte" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];
    
    memory.write_byte(reg[i_rd] , effAddr);
    
    debug << "sb: " << "memory[" << +i_rs << " + " << +i_i << "] = $" << +i_rd << endl;
    debug << hex << "HEX memory[" << effAddr << "] = " << reg[i_rs] << " + " << +i_i << endl;
    
    return;
}


void Simulator::sh()
{
    debug << "Store Half Word" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];
    
    memory.write_half_word(reg[i_rd], effAddr);
    
    debug << "sh: " << "memory[" << +i_rs << " + " << +i_i << "] = $" << +i_rd << endl;
    debug << hex << "HEX memory[" << effAddr << "] = " << reg[i_rs] << " + " << +i_i << endl;
    
    return;
}


void Simulator::slti()
{
     debug << "Set on Less Than Immediate" << endl;
     
     if((int32_t)reg[i_rs] < sign_extend_16(i_i))
     {
        reg[i_rd] = 1;
     }
     else
     {
        reg[i_rd] = 0;
     }
     
	 debug << "slti: " << "$" << +i_rd << " = " << "$" << +i_rs << " < " << +i_i << endl;
	 debug << dec << "DEC " << reg[i_rd] << " = " << reg[i_rs] << " < " << sign_extend_16(i_i) << endl;
	 
     return;
}


void Simulator::sltiu()
{
     debug << "Set on Less Than Immediate unsigned" << endl;
     
    uint32_t imm = sign_extend_16(i_i);

     if(reg[i_rs] < imm)
     {
        reg[i_rd] = 1;
     }
     else
     {
        reg[i_rd] = 0;
     }
	 debug << "sltiu: " << "$" << +i_rd << " = " << "$" << +i_rs << " < " << +imm << endl;
	 debug << dec << "DEC " << reg[i_rd] << " = " << reg[i_rs] << " < " << +imm << endl;
     
     return;
}

void Simulator::sw()
{
    debug << "Store Word" << endl;
    
    int32_t effAddr = sign_extend_16(i_i) + (int32_t)reg[i_rs];
    
    memory.write_word(reg[i_rd], effAddr);
    
    debug << "sw: " << "memory[$" << +i_rs << " + " << +i_i << "] = $" << +i_rd << endl;
    debug << hex << "HEX memory[" << effAddr << "] = " << reg[i_rs] << " + " << +i_i << endl;
    
    return;
}


void Simulator::xori()
{
    debug << "Exclusive Or Immediate" << endl;
	 
    int32_t result = reg[i_rs] ^ i_i;
    write_reg(i_rd, result);
     
    debug << "xori: " << "$" << +i_rd << " = $" << +i_rs << " ^ " << +i_i << endl;
    debug << hex << "HEX " << reg[i_rd] << " = " << reg[i_rs] << " ^ " << +i_i << endl;
	 
    return;
}
