#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "parser/parser.hpp"
#include "main.hpp"
#include "include/util.hpp"
#include "include/parserSimulatorAPI.hpp"
#include "simulator/include/simulator.hpp"

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

// std::vector<CompiledSimInput> compileSim(const std::string &rawCode)
// {
//     auto stream = stringToStream(rawCode);
//     ParsedLines ps;
//     parseLines(stream, ps);
//     return compileForSimulator(ps);
// }

RunInfo getRunInfo(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps);
    auto compiledSimInputs = compileForSimulator(ps);
    Simulator s(compiledSimInputs);
    auto ri = s.toRunInfo();
    return s.toRunInfo();
}