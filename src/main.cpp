#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "parser/parser.hpp"
#include "main.hpp"
#include "include/util.hpp"
#include "include/parserSimulatorAPI.hpp"
#include "simulator/include/simulator.hpp"


#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


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
    return ri;
}

std::string testBoost(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps);
    auto compiledSimInputs = compileForSimulator(ps);
    Simulator s(compiledSimInputs);
    auto ri = s.toRunInfo();
    std::ostringstream os;
    boost::archive::text_oarchive oa(os);
    oa << ri;
    return os.str();
}