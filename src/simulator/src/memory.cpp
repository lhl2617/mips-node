#include "../include/memory.hpp"
#include "../../include/defs.hpp"
#include "../../include/util.hpp"
#include <algorithm>
#include <string>
using namespace std;

Memory::Memory()
{
    reset();
}

void Memory::put_char(const uint32_t &word) const
{
    try
    {
        char c = word;
        debug << "Putchar: " << endl;
        putchar(c);
    }
    catch (const exception &e)
    {
        throwError(INTERNAL_ERROR, "I/O Error");
    }
}

uint32_t Memory::read_word_cin() const
{
    try
    {
        char ch;
        ch = getchar();

        if (ch != -1)
        {
            uint32_t result = (uint32_t)ch;
            result = (result & MASK_8_BIT);

            debug << "GETC: " << result << endl;
            return result;
        }
        else
        {
            return -1;
        }
    }
    catch (const exception &e)
    {
        throwError(INTERNAL_ERROR, "I/O Error");
    }
}

uint32_t Memory::read_word(const uint32_t &addr) const
{
    // GETC
    if (addr == ADDR_GETC)
    {
        return read_word_cin();
    }
    // Read from data memory
    else if (!(addr % 4) && addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH)
    {
        uint32_t mem_idx = addr - ADDR_DATA;
        return (getFromRWMemory(mem_idx + 3) | (getFromRWMemory(mem_idx + 2) << 8) | (getFromRWMemory(mem_idx + 1) << 16) | (getFromRWMemory(mem_idx) << 24));
    }
    // Read from instr memory
    else if (!(addr % 4) && addr >= ADDR_INSTR && addr < ADDR_INSTR + ADDR_INSTR_LENGTH)
    {
        uint32_t mem_idx = addr - ADDR_INSTR;
        if (mem_idx < instr_len)
        {
            return (instr_memory[mem_idx + 3]) | (instr_memory[mem_idx + 2] << 8) | (instr_memory[mem_idx + 1] << 16) | (instr_memory[mem_idx] << 24);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        throwError(MEMORY_EXCEPTION, "Out of range; Address tried: " + to_string(addr));
    }
}

void Memory::write_word(const uint32_t &word, const uint32_t &addr)
{
    // PUTC
    if (addr == ADDR_PUTC)
    {
        put_char(word);
    }
    // Write to memory
    else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH && !(addr % 4))
    {
        uint32_t mem_idx = addr - ADDR_DATA;
        rw_memory[mem_idx] = (word >> 24) & MASK_8_BIT;
        rw_memory[mem_idx + 1] = (word >> 16) & MASK_8_BIT;
        rw_memory[mem_idx + 2] = (word >> 8) & MASK_8_BIT;
        rw_memory[mem_idx + 3] = word & MASK_8_BIT;
    }
    else
    {
        throwError(MEMORY_EXCEPTION, "Out of range; Address tried: " + to_string(addr));
    }
}

uint32_t Memory::read_half_word(const uint32_t &addr) const
{
    // GETC
    if (addr == ADDR_GETC + 2)
    {
        return read_word_cin();
    }
    // correct alignment but read nothing unless EOF
    else if (addr == ADDR_GETC)
    {
        if (read_word_cin() == END_OF_FILE)
        {
            return 0x0000FFFF;
        }
        else
        {
            return 0;
        }
    }
    // Read from memory
    else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH && !(addr % 2))
    {
        uint32_t mem_idx = addr - ADDR_DATA;
        return (getFromRWMemory(mem_idx + 1)) | (getFromRWMemory(mem_idx) << 8);
    }
    // Read from data memory
    else if (!(addr % 2) && addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH)
    {
        uint32_t mem_idx = addr - ADDR_DATA;
        return (getFromRWMemory(mem_idx + 1)) | (getFromRWMemory(mem_idx) << 8);
    }
    // Read from instr memory
    else if (!(addr % 2) && addr >= ADDR_INSTR && addr < ADDR_INSTR + ADDR_INSTR_LENGTH)
    {
        uint32_t mem_idx = addr - ADDR_INSTR;
        if (mem_idx < instr_len)
        {
            return (instr_memory[mem_idx + 1]) | (instr_memory[mem_idx] << 8);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        throwError(MEMORY_EXCEPTION, "Out of range; Address tried: " + to_string(addr));
    }
}

void Memory::write_half_word(const uint32_t &word, const uint32_t &addr)
{
    // PUTC
    if (addr == ADDR_PUTC + 2)
    {
        put_char(word);
    }
    else if (addr == ADDR_PUTC)
    {
        put_char(0);
    }
    // Write to memory
    else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH && !(addr % 2))
    {
        uint32_t mem_idx = addr - ADDR_DATA;
        rw_memory[mem_idx] = (word >> 8) & MASK_8_BIT;
        rw_memory[mem_idx + 1] = word & MASK_8_BIT;
    }
    else
    {
        throwError(MEMORY_EXCEPTION, "Out of range; Address tried: " + to_string(addr));
    }
}

uint32_t Memory::read_byte(const uint32_t &addr) const
{
    // GETC
    if (addr == ADDR_GETC + 3)
    {
        return read_word_cin();
    }
    // correct alignment but read nothing unless EOF
    else if (addr >= ADDR_GETC && addr <= ADDR_GETC + 2)
    {
        if (read_word_cin() == END_OF_FILE)
        {
            return 0x000000FF;
        }
        else
        {
            return 0;
        }
    }
    // Read from data memory
    else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH)
    {
        uint32_t mem_idx = addr - ADDR_DATA;
        return getFromRWMemory(mem_idx);
    }
    // Read from instr memory
    else if (addr >= ADDR_INSTR && addr < ADDR_INSTR + ADDR_INSTR_LENGTH)
    {
        uint32_t mem_idx = addr - ADDR_INSTR;
        if (mem_idx < instr_len)
        {
            return instr_memory[mem_idx];
        }
        else
        {
            return 0;
        }
    }
    else
    {
        throwError(MEMORY_EXCEPTION, "Out of range; Address tried: " + to_string(addr));
    }
}

void Memory::write_byte(const uint32_t &word, const uint32_t &addr)
{
    // PUTC
    if (addr == ADDR_PUTC + 3)
    {
        put_char(word);
    }
    // correct alignment but write 0 unless EOF
    else if (addr >= ADDR_PUTC && addr <= ADDR_PUTC + 2)
    {
        put_char(0);
    }
    // Write to memory
    else if (addr >= ADDR_DATA && addr < ADDR_DATA + ADDR_DATA_LENGTH)
    {
        uint32_t mem_idx = addr - ADDR_DATA;
        rw_memory[mem_idx] = word & MASK_8_BIT;
    }
    else
    {
        throwError(MEMORY_EXCEPTION, "Out of range; Address tried: " + to_string(addr));
    }
}

void Memory::write_instr(const uint32_t &instr)
{
    if (instr_len < ADDR_INSTR_LENGTH)
    {
        instr_memory.push_back((instr >> 24) & MASK_8_BIT);
        instr_memory.push_back((instr >> 16) & MASK_8_BIT);
        instr_memory.push_back((instr >> 8) & MASK_8_BIT);
        instr_memory.push_back(instr & MASK_8_BIT);

        instr_len += 4;
    }
    else
    {
        throwError(MEMORY_EXCEPTION, "Program too large!");
    }
}

// redundant using read_word, but fewer branches
uint32_t Memory::read_instr(const uint32_t &pc) const
{
    if (!(pc % 4) && pc >= ADDR_INSTR)
    {
        uint32_t mem_idx = pc - ADDR_INSTR;
        if (mem_idx < instr_len)
        {
            uint32_t curr_inst = 0;

            curr_inst |= (instr_memory[mem_idx] << 24);
            curr_inst |= (instr_memory[mem_idx + 1] << 16);
            curr_inst |= (instr_memory[mem_idx + 2] << 8);
            curr_inst |= (instr_memory[mem_idx + 3]);

            return curr_inst;
        }
        else if (mem_idx < ADDR_INSTR_LENGTH)
        {
            return 0;
        }
        else
        {
            throwError(MEMORY_EXCEPTION, "Program counter out of range; address tried: " + to_string(pc));
        }
    }
    else
    {
        throwError(MEMORY_EXCEPTION, "PC not divisible by 4; Address tried: " + to_string(pc));
    }
}

// assumes already checked that it is in bounds, return 0 if not present in RWMem
uint8_t Memory::getFromRWMemory(const uint32_t &addr) const
{
    if (rw_memory.count(addr))
    {
        return rw_memory.at(addr);
    }
    return 0;
}

vector<uint32_t> Memory::getRWMemorySortedIndices() const
{
    vector<uint32_t> ret;
    ret.reserve(rw_memory.size());
    for (const auto &x : rw_memory)
        ret.push_back(x.first);
    sort(ret.begin(), ret.end());
    return ret;
}

void Memory::reserveInstrMem(const uint32_t &len)
{
    instr_memory.reserve(len);
}

void Memory::reset()
{
    rw_memory.clear();
    instr_memory.clear();
    instr_len = 0;
}