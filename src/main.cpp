#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "parser.hpp"
#include "main.hpp"

std::istringstream getCodeStream(const std::string &rawCode)
{
    std::istringstream out(rawCode);
    return out;
}

std::vector<std::vector<std::string>> getCommVector(const std::string &rawCode)
{
    std::istringstream codeStream = getCodeStream(rawCode);
    std::vector<std::vector<std::string>> commVector;
    vecParser(codeStream, commVector, false);
    return commVector;
}

/// entry: format
std::vector<std::string> format(const std::string &rawCode)
{
    auto commVector = getCommVector(rawCode);
    return formatterGen(commVector);
}

/// entry: parse
std::vector<uint32_t> parse(const std::string &rawCode)
{
    auto commVector = getCommVector(rawCode);
    return binGenVector(commVector);
}