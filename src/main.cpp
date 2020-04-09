#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "parser/parser.hpp"
#include "main.hpp"
#include "include/util.hpp"
#include "include/parserSimulatorAPI.hpp"
#include "simulator/include/simulator.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class TestClass
{
private:
    int a = 1;
    int b = 2;

    friend class boost::serialization::access;
    void serialize(boost::archive::binary_oarchive &ar, const unsigned int version)
    {
        ar &a;
        ar &b;
    }
};

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

RunInfo getRunInfo(const std::string &rawCode)
{
    auto stream = stringToStream(rawCode);
    ParsedLines ps;
    parseLines(stream, ps, false);
    auto compiledSimInputs = compileForSimulator(ps);
    Simulator s(compiledSimInputs);
    auto ri = s.toRunInfo();
    return ri;
}

std::string testBoost(const std::string &rawCode)
{
    // auto stream = stringToStream(rawCode);
    // ParsedLines ps;
    // parseLines(stream, ps, false);
    // auto compiledSimInputs = compileForSimulator(ps);
    // Simulator s(compiledSimInputs);
    // auto ri = s.toRunInfo();
    // // std::ostringstream os;
    // std::ofstream ofs("filename");
    // boost::archive::text_oarchive oa(ofs);
    // std::cout << '1';
    // oa << ri;
    // std::cout << '2';
    // // oa << ".";
    // // return os.str();
    // return "reeee";

    TestClass a;
    std::ofstream ofs("filename.txt", std::ios::binary);
    boost::archive::binary_oarchive ar(ofs);
    std::cout << '1';
    try
    {
        ar << a;
    }
    catch(boost::archive::archive_exception e)
    {
        cout << "what?";
    }
    // catch(...)
    // {
    //     cout << "UNCAUGHT";
    // }
    std::cout << '2';
    return "REEE";
}