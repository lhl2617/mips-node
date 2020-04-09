// API between parser and simulator

#ifndef PARSER_SIM_API_HPP
#define PARSER_SIM_API_HPP

/// this contains the compiled instruction in uint32_t format
/// and also the line number corresponding to that instruction
/// in the raw code
struct CompiledSimInput {
    uint32_t instr;
    uint32_t lineNo;
};


#endif