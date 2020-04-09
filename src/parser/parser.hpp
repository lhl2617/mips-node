#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctype.h>

#include "../include/parserSimulatorAPI.hpp"

using namespace std; // hack because isspace fails on msvs

typedef uint32_t (*commandFn)(std::vector<std::string> &argVec, int i);


struct numFn
{
    int numArgs;
    commandFn fn;
};

struct ParsedLine {
    std::vector<std::string> comm;
    unsigned int lineNo;  
    bool isComment = false;
};
typedef std::vector<ParsedLine> ParsedLines;

bool validIntStr(std::string arg, int32_t &returnVal, const int &pc = 0);
void pushInVec(ParsedLines &commVector, std::vector<std::string> &inVec, unsigned int &count, std::string &remain, const int &lineNo);
void parseLine(
    std::string &lineStr,
    ParsedLines &commVector,
    const unsigned int &lineNo,
    unsigned int &count,
    const bool &formatMode);
void parseLines(std::istream &inStream, ParsedLines &commVector, const bool& formatMode);
std::vector<uint32_t> compileParsedLines(ParsedLines &commVector);
std::vector<string> formatParsedLines(ParsedLines &commVector);
std::vector<CompiledSimInput> compileForSimulator(ParsedLines &commVector);

bool labelReturn(std::string str, int32_t &addr);


#endif //PARSER_HPP