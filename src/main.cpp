#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "parser/parser.hpp"
#include "main.hpp"
#include "include/util.hpp"
#include "include/parserSimulatorAPI.hpp"
#include "simulator/include/simulator.hpp"
#include "simulator/include/UI.hpp"

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

/// entry: intellisense
void intellisense(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps, false);
    compileParsedLines(ps); // compile it to let it run to exitError
}

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

Payload stepCode(const std::string &pickledRI, const int &steps)
{
    RunInfo ri;
    ri.unpickle(pickledRI);
    Simulator s(ri);
    s.stepCode(steps);
    auto endingRi = s.toRunInfo();

    auto pickledRIEnd = endingRi.pickle();
    auto vp = toVSCodePayload(endingRi);
    auto up = toUIPayload(endingRi);
    Payload ret = {
        pickledRIEnd,
        vp,
        up
    };

    return ret;
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