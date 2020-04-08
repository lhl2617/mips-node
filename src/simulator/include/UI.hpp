#ifndef UI_HPP
#define UI_HPP

#include <vector>
#include <string>
#include "../../include/util.hpp"
#include "../include/simulator.hpp"

struct VSCodePayload
{
    unsigned int highlightLineNo;

    std::string highlightClassName;
    //     | HighlightNormal // editor-line-highlight
    //     | HighlightNext // editor-line-highlight-next
    //     | HighlightError // editor-line-highlight-error

    std::string notificationMsg;
    std::string notificationType;
    // info/error/warning

};

struct UIPayload
{
    std::vector<std::pair<std::string, uint32_t>> regs;
    std::vector<std::pair<uint32_t, uint8_t>> mem; // addr, val
    uint32_t stepsDone;
    std::string statusBarMsg;

    std::string statusBarClassName;
    // positive, negative, primary
};

VSCodePayload toVSCodePayload(const RunInfo &ri);
UIPayload toUIPayload(const RunInfo &ri);

#endif