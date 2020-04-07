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

typedef std::vector<ParsedLine> ParsedLines;

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

bool validIntStr(std::string arg, int32_t &returnVal);
void pushInVec(ParsedLines &commVector, std::vector<std::string> &inVec, unsigned int &count, std::istringstream &remain, const int &lineNo);
void parseLines(std::istream &inStream, ParsedLines &commVector);
std::vector<uint32_t> compileParsedLines(ParsedLines &commVector);
std::vector<string> formatParsedLines(ParsedLines &commVector);
std::vector<CompiledSimInput> compileForSimulator(ParsedLines &commVector);

bool labelReturn(std::string str, int32_t &addr);


#endif //PARSER_HPP