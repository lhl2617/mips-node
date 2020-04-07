#include "../include/memory.hpp"
#include "../include/defs.hpp"
using namespace std;

Memory::Memory() {
	// exploration: use unordered_map to implement ADDR_DATA,
	// so no need to alloc full vector in beginning
	// O(1) access each byte read. Is it worth it? 
	// Less auxiliary memory, more runtime.
	rw_memory = vector<uint8_t>(ADDR_DATA_LENGTH, 0);
}

void Memory::put_char(const uint32_t& word) const {
	try {
		char c = word;
		debug << "Putchar: " << endl;
		putchar(c);
	}
	catch (const exception &e) {
		cerr << "I/O Error (-21)" << endl;
		exit(INTERNAL_ERROR);
	}
}

uint32_t Memory::read_word_cin() const {
	try {
		char ch;
		ch = getchar();

		if (ch != -1) {
			uint32_t result = (uint32_t) ch;
			result = (result & MASK_8_BIT);
			
			debug << "GETC: " << result << endl;
			return result;
		}
		else {
			return -1;
		}
	}
	catch (const exception &e) {
		cerr << "I/O Error (-21)" << endl;
		exit(INTERNAL_ERROR);
	}
	
}

uint32_t Memory::read_word(const uint32_t& addr) const {
	// GETC
	if (addr == ADDR_GETC) {
		return read_word_cin();
	}
	// Read from data memory
	else if (!(addr % 4) && addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH) {
		uint32_t mem_idx = addr - ADDR_DATA;
		return (rw_memory[mem_idx + 3]) | (rw_memory[mem_idx + 2] << 8) | (rw_memory[mem_idx + 1] << 16) | (rw_memory[mem_idx] << 24); 
	}
	// Read from instr memory
	else if (!(addr % 4) && addr >= ADDR_INSTR && addr < ADDR_INSTR + ADDR_INSTR_LENGTH) {
		uint32_t mem_idx = addr - ADDR_INSTR;
		if (mem_idx < instr_len) {
			return (instr_memory[mem_idx + 3]) | (instr_memory[mem_idx + 2] << 8) | (instr_memory[mem_idx + 1] << 16) | (instr_memory[mem_idx] << 24); 
		}
		else {
			return 0;
		}
	}
	else {
		debug << "Memory Exception (-11): Out of range" << endl;
		debug << "Address tried: " << addr << endl;
		exit(MEMORY_EXCEPTION);
	}	
}

void Memory::write_word(const uint32_t& word, const uint32_t& addr) {
	// PUTC
	if (addr == ADDR_PUTC) {
		put_char(word);
	}
	// Write to memory
	else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH && !(addr % 4)) {
		uint32_t mem_idx = addr - ADDR_DATA;
		rw_memory[mem_idx] = (word >> 24) & MASK_8_BIT;
		rw_memory[mem_idx + 1] = (word >> 16) & MASK_8_BIT;
		rw_memory[mem_idx + 2] = (word >> 8) & MASK_8_BIT;
		rw_memory[mem_idx + 3] = word & MASK_8_BIT;
	}
	else {
		debug << "Memory Exception (-11): Out of range" << endl;
		debug << "Address tried: " << addr << endl;
		exit(MEMORY_EXCEPTION);
	}	
}
		
uint32_t Memory::read_half_word(const uint32_t& addr) const {
	// GETC
	if (addr == ADDR_GETC + 2) {
		return read_word_cin();
	}
	// correct alignment but read nothing unless EOF
	else if (addr == ADDR_GETC) {
		if (read_word_cin() == END_OF_FILE) {
			return 0x0000FFFF;
		}
		else {
			return 0;
		}
	}
	// Read from memory
	else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH && !(addr % 2)) {
		uint32_t mem_idx = addr - ADDR_DATA;
		return (rw_memory[mem_idx + 1]) | (rw_memory[mem_idx] << 8); 
	}
	// Read from data memory
	else if (!(addr % 2) && addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH) {
		uint32_t mem_idx = addr - ADDR_DATA;
		return (rw_memory[mem_idx + 1]) | (rw_memory[mem_idx] << 8); 
	}
	// Read from instr memory
	else if (!(addr % 2) && addr >= ADDR_INSTR && addr < ADDR_INSTR + ADDR_INSTR_LENGTH) {
		uint32_t mem_idx = addr - ADDR_INSTR;
		if (mem_idx < instr_len) {
			return (instr_memory[mem_idx + 1]) | (instr_memory[mem_idx] << 8); 
		}
		else {
			return 0;
		}
	}
	else {
		debug << "Memory Exception (-11): Out of range" << endl;
		debug << "Address tried: " << addr << endl;
		exit(MEMORY_EXCEPTION);
	}
}

void Memory::write_half_word(const uint32_t& word, const uint32_t& addr) {
	// PUTC
	if (addr == ADDR_PUTC + 2) {
		put_char(word);
	}
	else if (addr == ADDR_PUTC) {
		put_char(0);
		
	}
	// Write to memory
	else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH && !(addr % 2)) {
		uint32_t mem_idx = addr - ADDR_DATA;
		rw_memory[mem_idx] = (word >> 8) & MASK_8_BIT;
		rw_memory[mem_idx + 1] = word & MASK_8_BIT;
	}
	else {
		debug << "Memory Exception (-11): Out of range" << endl;
		debug << "Address tried: " << addr << endl;
		exit(MEMORY_EXCEPTION);
	}	
}

uint32_t Memory::read_byte(const uint32_t& addr) const {
	// GETC
	if (addr == ADDR_GETC + 3) {
		return read_word_cin();
	}
	// correct alignment but read nothing unless EOF
	else if (addr >= ADDR_GETC && addr <= ADDR_GETC + 2) {
		if (read_word_cin() == END_OF_FILE) {
			return 0x000000FF;
		}
		else {
			return 0;
		}
	}
	// Read from data memory
	else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH) {
		uint32_t mem_idx = addr - ADDR_DATA;
		return rw_memory[mem_idx]; 
	}
	// Read from instr memory
	else if (addr >= ADDR_INSTR && addr < ADDR_INSTR + ADDR_INSTR_LENGTH) {
		uint32_t mem_idx = addr - ADDR_INSTR;
		if (mem_idx < instr_len) {
			return instr_memory[mem_idx]; 
		}
		else {
			return 0;
		}
	}
	else {
		debug << "Memory Exception (-11): Out of range" << endl;
		debug << "Address tried: " << addr << endl;
		exit(MEMORY_EXCEPTION);
	}
}

void Memory::write_byte(const uint32_t& word, const uint32_t& addr) {
	// PUTC
	if (addr == ADDR_PUTC + 3) {
		put_char(word);
	}
	// correct alignment but write 0 unless EOF
	else if (addr >= ADDR_PUTC && addr <= ADDR_PUTC + 2) {
		put_char(0);
	}
	// Write to memory
	else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH) {
		uint32_t mem_idx = addr - ADDR_DATA;
		rw_memory[mem_idx] = word & MASK_8_BIT;
	}
	else {
		debug << "Memory Exception (-11): Out of range" << endl;
		debug << "Address tried: " << addr << endl;
		exit(MEMORY_EXCEPTION);
	}	
}

// write binary file input into memory (byte by byte)
void Memory::write_instr(const uint8_t& byte_instr) {
	if (instr_len < ADDR_INSTR_LENGTH) {
		instr_memory.push_back(byte_instr);
		instr_len++;
	}
	else {
		debug << "Stopped writing: Binary file too large!" << endl;
		// exit(MEMORY_EXCEPTION);
		// shouldn't exit, just stop writing 
	}	
}

// redundant using read_word, but fewer branches
uint32_t Memory::read_instr(const uint32_t& pc) const {
	if (!(pc % 4) && pc >= ADDR_INSTR) {
		uint32_t mem_idx = pc - ADDR_INSTR;
		if (mem_idx < instr_len) {
			uint32_t curr_inst = 0;
				  
			curr_inst |= (instr_memory[mem_idx] << 24);
			curr_inst |= (instr_memory[mem_idx + 1] << 16);
			curr_inst |= (instr_memory[mem_idx + 2] << 8);
			curr_inst |= (instr_memory[mem_idx + 3]);
			
			return curr_inst;
		}
		else if (mem_idx < ADDR_INSTR_LENGTH) {
			return 0;
		}
		else {
			debug << "PC invalid: ";
			debug << "PC out of range" << endl;
			debug << "Memory Exception (-11)" << endl;
			debug << "Address tried: " << pc << endl;
			exit(MEMORY_EXCEPTION);
		}
	}
	else {
		debug << "PC invalid: ";
		debug << "PC not divisible by 4" << endl;
		debug << "Memory Exception (-11)" << endl;
		debug << "Address tried: " << pc << endl;
		exit(MEMORY_EXCEPTION);
	}
}
	
