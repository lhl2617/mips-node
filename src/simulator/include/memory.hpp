#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <cstdint>
#include <unordered_map>

using namespace std;

class Memory
{
public:
    Memory();

    void put_char(const uint32_t &word) const;
    uint32_t read_word_cin() const;

    uint32_t read_word(const uint32_t &addr) const;
    void write_word(const uint32_t &word, const uint32_t &addr);

    uint32_t read_half_word(const uint32_t &addr) const;
    void write_half_word(const uint32_t &word, const uint32_t &addr);

    uint32_t read_byte(const uint32_t &addr) const;
    void write_byte(const uint32_t &word, const uint32_t &addr);

    void write_instr(const uint8_t &byte_instr);
    uint32_t read_instr(const uint32_t &pc) const;

    uint8_t getFromRWMemory(const uint32_t &addr) const;

    vector<uint32_t> getRWMemorySortedIndices() const;

    void reserveInstrMem(const uint32_t &len);

    void reset();

private:
    unordered_map<uint32_t, uint8_t> rw_memory;
    vector<uint8_t> instr_memory;
    uint32_t instr_len;
};

#endif
