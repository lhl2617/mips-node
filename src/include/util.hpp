#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "./defs.hpp"

using namespace std;

struct retException
{
    int ret;
    std::string msg;
};

struct RunErr
{
    bool error = false;
    string header = "";
    string message = "";

    
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &error;
        ar &header;
        ar &message;
    }
};

// for parser and formatter part
inline void exitError(std::string err, int line, int code)
{
    std::string errMsg = std::to_string(line) + " ----- " + err;
    throw retException({code, errMsg});
}

// for simulator
inline void throwError(const int &code, const std::string &msg)
{
    std::string header;
    if (code == ARITHMETIC_EXCEPTION)
        header = "ARITHMETIC_EXCEPTION";
    else if (code == MEMORY_EXCEPTION)
        header = "MEMORY_EXCEPTION";
    else if (code == INVALID_INSTRUCTION)
        header = "INVALID_INSTRUCTION";
    else if (code == INTERNAL_ERROR)
        header = "INTERNAL_ERROR";
    else if (code == IO_ERROR)
        header = "IO_ERROR";
    else
        header = "UNKNOWN_ERROR";

    throw RunErr({ true, header, msg });
}

inline std::istringstream stringToStream(const std::string &s)
{
    std::istringstream out(s);
    return out;
}

inline std::string trim(const std::string &s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) { return isspace(c); });
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) { return isspace(c); }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

#endif