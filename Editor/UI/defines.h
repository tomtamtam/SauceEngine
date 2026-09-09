#pragma once

#include <cstdio>
#include <nlohmann/json.hpp>

#include <string>
#include <unordered_map>

#include "Editor/UI/ImGuiLayer.h"

#include "Engine/vendor/utils_hpp/escapes.hpp"

using json = nlohmann::json;

const ImGuiWindowFlags hostFlags =
     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
     ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoMove |
     ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
     ImGuiWindowFlags_MenuBar;

const ImGuiWindowFlags floatingFlags =
     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
     ImGuiWindowFlags_NoResize;

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

    void put(MessageType type, const std::string text)
    {
        messages.push_back({type, text});
        if(type == MessageType::WARNING && !showWarnings)
            return;
        PrintMsg(type, text);
    }

    static void PrintMsg(MessageType t, const std::string &s)
    {
        std::printf("%s %s\n", Escapes::ColoredText(NameMessageMap.at(t), ColorMessageMap.at(t)).c_str(), s.c_str());
    }

    void copyAll()
    {}
    std::vector<Message> messages;

    Console()
        : autoScroll(true), showWarnings(true)
    {
    }
    ~Console() {}
};
