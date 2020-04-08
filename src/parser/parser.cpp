#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <iomanip>
#include <sstream>
#include "parser.hpp"
#include "instructionList.hpp"
#include "../include/parserSimulatorAPI.hpp"
#include "../include/util.hpp"
#include "../include/defs.hpp"

std::unordered_map<std::string, numFn> commMap = {
    {"add", {3, add}},
    {"addu", {3, addu}},
    {"and", {3, and_instr}},
    {"div", {2, div_instr}},
    {"divu", {2, divu}},
    {"jr", {1, jr}},
    {"jalr", {2, jalr}},
    {"mfhi", {1, mfhi}},
    {"mflo", {1, mflo}},
    {"mthi", {1, mthi}},
    {"mtlo", {1, mtlo}},
    {"mult", {2, mult}},
    {"multu", {2, multu}},
    {"or", {3, or_instr}},
    {"sll", {3, sll}},
    {"sllv", {3, sllv}},
    {"slt", {3, slt}},
    {"sltu", {3, sltu}},
    {"sra", {3, sra}},
    {"srav", {3, srav}},
    {"srl", {3, srl}},
    {"srlv", {3, srlv}},
    {"sub", {3, sub}},
    {"subu", {3, subu}},
    {"xor", {3, xor_instr}},

    {"addi", {3, addi}},
    {"addiu", {3, addiu}},
    {"andi", {3, andi}},
    {"beq", {3, beq}},
    {"bgezal", {2, bgezal}},
    {"bgez", {2, bgez}},
    {"bgtz", {2, bgtz}},
    {"blez", {2, blez}},
    {"bltz", {2, bltz}},
    {"bltzal", {2, bltzal}},
    {"bne", {3, bne}},
    {"ori", {3, ori}},
    {"slti", {3, slti}},
    {"sltiu", {3, sltiu}},
    {"xori", {3, xori}},
    {"lb", {2, lb}},
    {"lbu", {2, lbu}},
    {"lh", {2, lh}},
    {"lhu", {2, lhu}},
    {"lui", {2, lui}},
    {"lw", {2, lw}},
    {"lwl", {2, lwl}},
    {"lwr", {2, lwr}},
    {"sb", {2, sb}},
    {"sh", {2, sh}},
    {"sw", {2, sw}},

    {"j", {1, j}},
    {"jal", {1, jal}},

    {"nop", {0, nop}}};

std::map<std::string, unsigned int> labelMap;

bool validIntStr(std::string arg, int32_t &returnVal)
{
    std::size_t pos;
    returnVal = std::stoi(arg, &pos, 0);
    if (pos != arg.length())
    {
        return false;
    }
    return true;
}

void pushInVec(ParsedLines &commVector, std::vector<std::string> &inVec, unsigned int &count, std::istringstream &remain, const unsigned int &lineNo)
{
    // check for comment in remain
    std::string remainTok;
    std::string remainStr = remain.str();
    if (remain >> remainTok)
    {
        if (remainTok[0] == '#')
        {
            inVec.push_back("# " + remainTok.substr(1) + trim(remain.str()));
        }
        else
        {
            exitError("Too many arguments: \"" + remainStr + "\"", lineNo, 5);
        }
    }

    ParsedLine p = { inVec, lineNo };
    commVector.push_back(p);
    count += 4;
}

/// parse an incoming string into a vector of ParsedLines
void parseLines(std::istream &inStream, ParsedLines &commVector)
{
    unsigned int count = ADDR_INSTR;
    unsigned int lineNo = 0;
    std::string lineStr;
    std::string func;
    while (getline(inStream, lineStr))
    {
        std::istringstream line = stringToStream(lineStr);
        line >> func;
        if (func == "exit")
            return;
        else if (func[0] == '#')
        {
            // whole line is a comment
            ParsedLine p = { {"", "# " + func.substr(1) + trim(line.str())}, lineNo, true };
            commVector.push_back(p);
        }
        else if (func.back() == ':')
        {
            func.pop_back();
            labelMap[func] = count;
        }
        else if (!commMap.count(func))
        {
            exitError("Invalid instruction \"" + func + "\"", lineNo, 5);
        }
        else
        {
            std::vector<std::string> inVec;
            if (func == "jalr")
            {
                std::string arg1, arg2;
                if (!(line >> arg1))
                    exitError("Not enough arguments for final instruction.", lineNo, 5);
                if (!(line >> arg2))
                {
                    inVec.push_back(func);
                    inVec.push_back("$31");
                    inVec.push_back(arg1);
                    pushInVec(commVector, inVec, count, line, lineNo);
                }
                if (indRegCheck(arg2))
                {
                    inVec.push_back(func);
                    inVec.push_back(arg1);
                    inVec.push_back(arg2);
                    pushInVec(commVector, inVec, count, line, lineNo);
                }
                else
                {
                    inVec.push_back(func);
                    inVec.push_back("$31");
                    inVec.push_back(arg1);
                    pushInVec(commVector, inVec, count, line, lineNo);

                    /// won't be used -- now we are governed by lines
                    // return addVec(inStream, commVector, count, arg2);
                }
            }
            else
            {
                inVec.push_back(func);
                for (int i = 0; i < commMap[func].numArgs; ++i)
                {
                    std::string arg;
                    if (!(line >> arg))
                        exitError("Not enough arguments for final instruction.", lineNo, 5);
                    inVec.push_back(arg);
                }
                pushInVec(commVector, inVec, count, line, lineNo);
            }
            ++lineNo;
        }
    }
}

std::vector<uint32_t> compileParsedLines(ParsedLines &commVector)
{
    std::vector<uint32_t> out;
    for (int i = 0; i < commVector.size(); ++i)
    {
        auto &x = commVector[i];
        if (!x.isComment)
        {
            uint32_t val = commMap[x.comm[0]].fn(x.comm, i);
            out.push_back(val);
        }
    }
    return out;
}

std::vector<string> formatParsedLines(ParsedLines &commVector)
{
    std::vector<std::string> out;
    for (auto &x : commVector)
    {
        std::stringstream ss;

        for (std::string &elem : x.comm)
        {
            ss << std::left << std::setw(8) << elem;
        }
        out.push_back(ss.str());
    }
    return out;
}

std::vector<CompiledSimInput> compileForSimulator(ParsedLines &commVector)
{
    std::vector<CompiledSimInput> out;
    for (int i = 0; i < commVector.size(); ++i)
    {
        auto &x = commVector[i];
        if (!x.isComment)
        {
            uint32_t instr = commMap[x.comm[0]].fn(x.comm, i);

            out.push_back({ instr, x.lineNo });
        }
    }
    return out;
}

bool labelReturn(std::string str, int32_t &addr)
{
    if (labelMap.find(str) == labelMap.end())
        return false;
    addr = labelMap[str];
    return true;
}