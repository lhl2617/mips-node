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
    parseLines(stream, ps, true);
    return formatParsedLines(ps);
}

/// entry: parse
std::vector<uint32_t> compile(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps, false);
    return compileParsedLines(ps);
}

// std::vector<CompiledSimInput> compileSim(const std::string &rawCode)
// {
//     auto stream = stringToStream(rawCode);
//     ParsedLines ps;
//     parseLines(stream, ps);
//     return compileForSimulator(ps);
// }

std::string getPickledRunInfo(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps, false);
    auto compiledSimInputs = compileForSimulator(ps);
    Simulator s(compiledSimInputs);
    auto ri = s.toRunInfo();
    return ri.pickle();
}

std::string testBoost(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps, false);
    auto compiledSimInputs = compileForSimulator(ps);
    Simulator s(compiledSimInputs);
    auto ri = s.toRunInfo();
    ri.regHI = 69;
    // std::ostringstream os;
    // // std::ofstream ofs("filename");
    // boost::archive::text_oarchive oa(os);
    // oa << ri;
    return ri.pickle();
}