// UI functions

#include <vector>
#include <string>
#include "../../include/util.hpp"
#include "../include/UI.hpp"
#include "../../include/defs.hpp"

inline std::pair<std::string, std::string> getVSCodeNotificationPair(const RunInfo &ri)
{
    if (ri.runErr.error)
    {
        std::string errMsg = ri.runErr.header + ": " + ri.runErr.message;
        return make_pair(errMsg, std::string("error"));
    }
    return make_pair(std::string(), std::string());
}

VSCodePayload toVSCodePayload(const RunInfo &ri)
{
    auto notificationPair = getVSCodeNotificationPair(ri);
    VSCodePayload ret =
        {
            ri.lineMap[ri.pc - ADDR_INSTR],
            "editor-line-highlight",
            notificationPair.first,
            notificationPair.second};
    return ret;
}

inline std::vector<std::pair<std::string, uint32_t>> getRegsUI(const RunInfo &ri)
{
    std::vector<std::pair<std::string, uint32_t>> ret;
    ret.reserve(ri.reg.size() + 2);

    for (int i = 0; i < ri.reg.size(); ++i)
    {
        ret.push_back(make_pair("$" + to_string(i), ri.reg[i]));
    }
    ret.push_back(make_pair(std::string("$LO"), ri.regLO));
    ret.push_back(make_pair(std::string("$HI"), ri.regHI));

    return ret;
}

inline std::vector<std::pair<uint32_t, uint8_t>> getMemUI(const Memory &mem)
{
    // only get those with non-zero mem
    std::vector<std::pair<uint32_t, uint8_t>> ret;
    vector<uint32_t> indices = mem.getRWMemorySortedIndices();
    ret.reserve(indices.size());

    for (auto &x : indices)
    {
        std::pair<uint32_t, uint8_t> p = make_pair(x, (uint8_t)mem.read_byte(x));
        ret.push_back(p);
    }
    return ret;
}

inline std::pair<std::string, std::string> getStatusBarPair(const RunInfo &ri)
{
    if (ri.runErr.error)
    {
        return make_pair(ri.runErr.header, std::string("negative"));
    }
    switch (ri.pc)
    {
    case ADDR_NULL:
        return make_pair(std::string("Execution Complete"), std::string("positive"));
    default:
        return make_pair(std::string("Stepping"), std::string("primary"));
    }
    return make_pair(std::string("-"), std::string());
}

UIPayload toUIPayload(const RunInfo &ri)
{
    auto regs = getRegsUI(ri);
    auto mem = getMemUI(ri.memory);
    std::vector<std::pair<uint32_t, uint32_t>> mem; // addr, val
    auto statusBarPair = getStatusBarPair(ri);
    std::string &statusBarMsg = statusBarPair.first;
    std::string &statusBarClassName = statusBarPair.second;

    UIPayload ret = {
        regs,
        mem,
        ri.stepsDone,
        statusBarMsg,
        statusBarClassName};

    return ret;
}