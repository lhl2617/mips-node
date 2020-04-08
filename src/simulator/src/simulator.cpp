#include <iomanip>

#include "../include/simulator.hpp"
#include "../../include/defs.hpp"

using namespace std;

// uint32_t SIMULATOR_HISTORY_MAX_LENGTH = 10;
// uint32_t MAX_RUN_STEPS = 5000;

Simulator::Simulator()
{
    reset();
}

Simulator::Simulator(const vector<CompiledSimInput> &v)
{
    reset();

    memory.reserveInstrMem(v.size());
    lineMap.reserve(v.size());
    for (auto &x : v)
    {
        memory.write_instr(x.instr);
        lineMap.push_back(x.lineNo);
    }
}

Simulator::Simulator(const RunInfo &ri)
{
    assignRunInfo(ri);
}

void Simulator::run_inst()
{
    opcode = curr_inst >> 26;
    if (opcode == 0)
    { // r-type
        r_rs = (curr_inst >> 21) & MASK_5_BIT;
        r_rt = (curr_inst >> 16) & MASK_5_BIT;
        r_rd = (curr_inst >> 11) & MASK_5_BIT;
        r_sa = (curr_inst >> 6) & MASK_5_BIT;
        r_fncode = (curr_inst)&MASK_6_BIT;

        call_r(r_fncode);
    }
    else if (opcode == 2 || opcode == 3)
    { // j-type
        j_i = curr_inst & MASK_26_BIT;

        call_j(opcode);
    }
    else
    { // i-type
        i_rs = (curr_inst >> 21) & MASK_5_BIT;
        i_rd = (curr_inst >> 16) & MASK_5_BIT;
        i_i = (curr_inst)&MASK_16_BIT;

        call_i(opcode);
    }
}

void Simulator::updateHistory()
{
    // make sure history length within bounds
    if (history.size() == 10)
        history.pop_front();

    history.push_back(toRunInfo());
}

RunInfo Simulator::toRunInfo() const
{
    RunInfo r = {
        memory,
        reg,
        regHI,
        regLO,
        pc,
        stepsDone,
        history,
        lineMap,
        runErr};
    return r;
}

void Simulator::assignRunInfo(const RunInfo &ri)
{
    memory = ri.memory;
    reg = ri.reg;
    regHI = ri.regHI;
    regLO = ri.regLO;
    pc = ri.pc;
    stepsDone = ri.stepsDone;
    history = ri.history;
    lineMap = ri.lineMap;
}

// exposed
void Simulator::stepFwdBy(const uint32_t &steps)
{
    uint32_t target = stepsDone + steps;
    try
    {
        while ((!steps || stepsDone < target) && pc != ADDR_NULL && !runErr.error)
        {
            stepFwd(target);
        }
    }
    catch (RunErr &e)
    {
        runErr = e;
    }
}

void Simulator::stepFwd(const uint32_t &target)
{
    if (pc == ADDR_NULL || runErr.error)
    {
        // do nothing
    }
    else
    {
        // save to history if within history length of target, if target known
        // if target == 0 then save history
        if (!target || target - stepsDone < 10)
            updateHistory();
        ++stepsDone;

        curr_inst = memory.read_instr(pc);
        run_inst();

        // update pc, based on delayed branch
        if (branch)
        {
            pc = branch_addr;
            branch = false;
        }
        else if (call_branch)
        {
            pc += 4;
            call_branch = false;
            branch = true;
        }
        else
        {
            pc += 4;
        }
    }
}

// exposed
void Simulator::stepBwd()
{
    if (history.size())
    {
        auto ret = history.front();
        history.pop_front();
        assignRunInfo(ret);
    }
    else
    {
        // we need to run code form reset until stepsDone - 1
        uint32_t target = stepsDone - 1;
        reset();
        try
        {
            while (stepsDone <= target)
            {
                stepFwd(target);
            }
        }
        catch (RunErr &e)
        {
            runErr = e;
        }
    }
}

void Simulator::reset()
{
    memory.reset();
    reg = std::vector<uint32_t>(32, 0);
    regHI = 0;
    regLO = 0;
    pc = ADDR_INSTR;
    stepsDone = 0;
    history.clear();
    lineMap.clear();

    RunErr r;
    runErr = r;
}

// write registers from $1 - $31
void Simulator::write_reg(const int &idx, const uint32_t &val)
{
    if (idx == 0)
    {
        debug << "Cannot write to $zero, write ignored." << endl;
        return;
    }
    reg[idx] = val;
    return;
}
