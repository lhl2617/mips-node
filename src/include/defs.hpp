#ifndef MIPSINCLUDE_HPP
#define MIPSINCLUDE_HPP

#include <bitset>
#include <iostream>

#define LOG_DEBUG 1 // enable debug
#define LOG_INSTR 0 // print out instruction as binary

#define debug if (!LOG_DEBUG) {} else std::cerr

#define log8(x) if (!LOG_INSTR) {} else { bitset<8> a(x); std::cerr << a << endl; }
		
#define ADDR_NULL 0x00000000
#define ADDR_INSTR 0x10000000 
#define ADDR_DATA 0x20000000
#define ADDR_GETC 0x30000000
#define ADDR_PUTC 0x30000004

#define ADDR_DATA_LENGTH 0x4000000
#define ADDR_INSTR_LENGTH 0x1000000

#define MASK_5_BIT 0x1F
#define MASK_6_BIT 0x3F
#define MASK_8_BIT 0xFF
#define MASK_16_BIT 0xFFFF
#define MASK_26_BIT 0x3FFFFFF

#define ARITHMETIC_EXCEPTION -10
#define MEMORY_EXCEPTION -11
#define INVALID_INSTRUCTION -12
#define INTERNAL_ERROR -20
#define IO_ERROR -21

#define END_OF_FILE 0xFFFFFFFF

#endif
