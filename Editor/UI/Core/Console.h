#pragma once

#include <unordered_map>
#include <vector>

#include "utils_hpp/escapes.hpp"

enum class MessageType
{
    DEBUG,
    WARNING,
    ERROR,
    INFO
};

const std::unordered_map<MessageType, Escapes::Color> ColorMessageMap
{
    {MessageType::DEBUG, Escapes::BLUE},
    {MessageType::WARNING, Escapes::YELLOW},
    {MessageType::ERROR, Escapes::RED},
    {MessageType::INFO, Escapes::CYAN}
};

const std::unordered_map<MessageType, std::string> NameMessageMap
{
    {MessageType::DEBUG, "DEBUG:"},
    {MessageType::WARNING, "WARNING:"},
    {MessageType::ERROR, "ERROR:"},
    {MessageType::INFO, "INFO:"}
};

struct Message
{
    MessageType type;
    std::string text;
};

class Console
{
public:
    bool autoScroll;
    bool showWarnings;
    void clear()
    { messages.clear(); }

    void Put(MessageType type, const std::string text)
    {
        messages.push_back({type, text});
        if(type == MessageType::WARNING && !showWarnings)
            return;
        PrintMsg(type, text);
    }

    void copyAll()
    {}
    std::vector<Message> messages;

    Console()
        : autoScroll(true), showWarnings(true)
    {
    }
    ~Console() {}

private:
    static void PrintMsg(MessageType t, const std::string &s)
    {
        std::printf("%s %s\n", Escapes::ColoredText(NameMessageMap.at(t), ColorMessageMap.at(t)).c_str(), s.c_str());
    }
};
