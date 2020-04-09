#ifndef MAIN_HPP
#define MAIN_HPP

#include <vector>
#include <string>
#include <sstream>
#include "parser/parser.hpp"
#include "simulator/include/simulator.hpp"
#include "simulator/include/UI.hpp"

// interface to node
struct Payload
{
    std::string ri; // pickled runinfo
    VSCodePayload v;
    UIPayload u;
};

/// entry: format
std::vector<std::string> format(const std::string &rawCode);
/// entry: parse
std::vector<uint32_t> compile(const std::string &rawCode);

/// get initial runInfo string, performs checks too
std::string getPickledRunInfo(const std::string &rawCode);

/// stepCode
Payload stepCode(const std::string &pickledRI, const int &steps);

std::string testBoost(const std::string &rawCode);
#endif