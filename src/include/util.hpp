#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include "simulator/include/simulator.hpp"

using namespace std;

struct retException
{
    int ret;
    std::string msg;
};

void exitError(std::string err, int line, int code)
{
    // std::cerr << errMsg << std::endl << std::endl;
    // std::exit(code);
    std::string errMsg = std::to_string(line) + " ----- " + err;
    throw retException({code, errMsg});
}

std::istringstream stringToStream(const std::string &s)
{
    std::istringstream out(s);
    return out;
}

std::string trim(const std::string &s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) { return isspace(c); });
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) { return isspace(c); }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

#endif