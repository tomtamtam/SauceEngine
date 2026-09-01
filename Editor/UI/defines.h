#pragma once

#include "Editor/UI/ImGuiLayer.h"
#include "Engine/vendor/utils_hpp/escapes.hpp"
#include "Input/InputSystem.h"
#include <cstdio>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

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

inline static Sauce::InputSystem inputSystem;

inline static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    inputSystem.KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;
}

inline static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    inputSystem.MouseButtonCallback(window, button, action, mods);
}

inline static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    inputSystem.ScrollCallback(window, xoffset, yoffset);
}

inline static void char_callback(GLFWwindow* window, unsigned int codepoint)
{
    ImGui_ImplGlfw_CharCallback(window, codepoint);
    inputSystem.CharCallback(window, codepoint);
}

inline static void enter_callback(GLFWwindow* window, int entered)
{
    ImGui_ImplGlfw_CursorEnterCallback(window, entered);
    inputSystem.EnterCallback(window, entered);
}

inline static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
    inputSystem.CursorPosCallback(window, xpos, ypos);
}

inline static void window_focus_callback(GLFWwindow* window, int focused)
{
    ImGui_ImplGlfw_WindowFocusCallback(window, focused);
    inputSystem.WindowFocusCallback(window, focused);
}

inline void on_close()
{}
