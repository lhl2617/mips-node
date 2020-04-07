#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctype.h>

using namespace std; // hack because isspace fails on msvs

typedef uint32_t (*commandFn)(std::vector<std::string> &argVec, int i);

struct retException
{
    int ret;
    std::string msg;
};

struct numFn
{
    int numArgs;
    commandFn fn;
};

bool validIntStr(std::string arg, int32_t &returnVal);
void vecParser(std::istream &inStream, std::vector<std::vector<std::string>> &commVector, const bool &throwComment = true);
bool addVec(std::istream &inStream, std::vector<std::vector<std::string>> &commVector, unsigned int &count, std::string func, const bool &throwComment = true);
void binGen(std::ofstream &outStream, std::vector<std::vector<std::string>> &commVector);
void fillMem(char memBlock[4], uint32_t num);
bool labelReturn(std::string str, int32_t &addr);

/// binary
std::vector<uint32_t> binGenVector(std::vector<std::vector<std::string>> &commVector);

/// formatter
std::vector<std::string> formatterGen(std::vector<std::vector<std::string>> &commVector);

inline std::string trim(const std::string &s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) { return isspace(c); });
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) { return isspace(c); }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

#endif //PARSER_HPP