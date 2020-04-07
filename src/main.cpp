#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "parser/parser.hpp"
#include "main.hpp"
#include "include/util.hpp"
#include "include/parserSimulatorAPI.hpp"

/// entry: format
std::vector<std::string> format(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps);
    return formatParsedLines(ps);
}

/// entry: parse
std::vector<uint32_t> compile(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps);
    return compileParsedLines(ps);
}

std::vector<CompiledSimInput> compileSim(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps);
    return compileForSimulator(ps);
}