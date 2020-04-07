#ifndef MAIN_HPP
#define MAIN_HPP

#include <vector>
#include <string>
#include <sstream>

#include "parser.hpp"

/// entry: format
std::vector<std::string> format(const std::string &rawCode);
/// entry: parse
std::vector<uint32_t> parse(const std::string &rawCode);

std::istringstream getCodeStream(const std::string &rawCode);
std::vector<std::vector<std::string>> getCommVector(const std::string &rawCode);

#endif