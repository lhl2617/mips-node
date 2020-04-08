#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <vector>
#include <string>
#include <fstream>
#include <list>

#include "memory.hpp"
#include "../../include/parserSimulatorAPI.hpp"
#include "../../include/util.hpp"

using namespace std;

// extern uint32_t SIMULATOR_HISTORY_MAX_LENGTH;
// extern uint32_t MAX_RUN_STEPS;

struct RunInfo
{
    Memory memory;
    vector<uint32_t> reg;
    uint32_t regHI;
    uint32_t regLO;
    uint32_t pc;
    uint32_t stepsDone;
    list<RunInfo> history;
    vector<uint32_t> lineMap;
    RunErr runErr;
};

class Simulator
{
    // typedef void (Simulator::*func_ptr)();

public:
    Simulator();
    Simulator(const vector<CompiledSimInput> &v);
    Simulator(const RunInfo &ri);
    // int init(const string &in_filename); // init instr into memory, return exit code

    // run from x steps from current stepsDone, if 0 run forever
    void stepFwdBy(const uint32_t &steps = 5000);
    
    // step back
    void stepBwd();
    // return currentInfo
    RunInfo toRunInfo() const;
    
private:
    // instantiate memory
    Memory memory;
    vector<uint32_t> reg; // registers
    uint32_t regHI;
    uint32_t regLO;
    uint32_t pc; // program counter
    uint32_t stepsDone;
    list<RunInfo> history;
    // this maps from PC-ADDR_INSTR to the line number
    vector<uint32_t> lineMap;
    RunErr runErr;


    uint32_t instr_idx; // idx of first byte of instruction in byte_inst
    uint32_t curr_inst; // current instruction
    uint32_t branch_addr;
    bool call_branch = false; // for delayed branch
    bool branch = false;

    uint8_t opcode;

    // stores operands
    uint8_t r_rs;
    uint8_t r_rt;
    uint8_t r_rd;
    uint8_t r_sa;
    uint8_t r_fncode;

    uint8_t i_rs;
    uint8_t i_rd;
    uint16_t i_i;

    uint32_t j_i;

    // write registers from $1 - $31
    void write_reg(const int &idx, const uint32_t &val);

    // FOR DEBUGGING
    uint32_t debugsrc;
    uint32_t debugsrc1;
    uint32_t debugsrc2;
    // step front - target is only used to optimise updateHistory
    void stepFwd(const uint32_t &target = 0);
    void assignRunInfo(const RunInfo &ri);

    void updateHistory();
    void reset();

    void read_inst(); // read next instruction into curr_inst
    void run_inst();  // decode curr_inst and run instruction

    // callers
    void call_r(uint8_t param);
    void call_i(uint8_t param);
    void call_j(uint8_t param);

    // r-type
    void add();
    void addu();
    void bitwise_and();
    void bitwise_or();
    void bitwise_xor();
    void div();
    void divu();
    void jr();
    void jalr();
    void mfhi();
    void mflo();
    void mthi();
    void mtlo();
    void mult();
    void multu();
    void sll();
    void sllv();
    void slt();
    void sltu();
    void sra();
    void srav();
    void srl();
    void srlv();
    void sub();
    void subu();

    // i-type
    void addi();
    void addiu();
    void andi();
    void beq();
    void bgez();
    void bgezal();
    void bgtz();
    void blez();
    void bltz();
    void bltzal();
    void bne();
    void lb();
    void lbu();
    void lh();
    void lhu();
    void lui();
    void lw();
    void lwl();
    void lwr();
    void ori();
    void sb();
    void sh();
    void slti();
    void sltiu();
    void sw();
    void xori();

    void call_cond_B();

    // j-type
    void j();
    void jal();

    // helper
    int32_t sign_extend_16(int16_t num);
    int32_t sign_extend_8(int8_t num);


    // debug
    // void getlogs();
    // string filename;
};

#endif
