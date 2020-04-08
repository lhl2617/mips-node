#ifndef MAIN_HPP
#define MAIN_HPP

#include <vector>
#include <string>
#include <sstream>
#include "parser/parser.hpp"
#include "simulator/include/simulator.hpp"


/// entry: format
std::vector<std::string> format(const std::string &rawCode);
/// entry: parse
std::vector<uint32_t> compile(const std::string &rawCode);
/// entry: stepCode

// test
RunInfo getRunInfo(const std::string &rawCode);

#endif