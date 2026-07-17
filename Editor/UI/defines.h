#pragma once

#include "Core/SceneSerializer.h"
#include "ECS/Scene.h"
#include "Render/Renderer.h"
#include "Editor/UI/ImGuiLayer.h"
#include <format>
#include <iostream>
#include <memory>
#include <vector>

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

inline std::string colored(const std::string &text, int a, int b)
{
    return std::format("\x1B[{}m{}\0{}[0m", a, text, b);
}

inline std::string coloredMsgEntry(MessageType type)
{

    switch (type)
    {
        case MessageType::ERROR:
            return colored("[Error]", 31, 41);
            break;
        case MessageType::INFO:
            return colored("[Info]", 36, 46);
            break;
        case MessageType::WARNING:
            return colored("[Warning]", 33, 43);
            break;
        default:
            return colored("[Debug]", 35, 45);
            break;
    }
}

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
        std::cout << coloredMsgEntry(type) << '\n' << text << '\n';
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

struct EditorState
{
    std::shared_ptr<Sauce::Renderer> renderer;
    std::shared_ptr<Sauce::Scene> scene;
    Sauce::SceneSerializer sceneSerializer;
    ImGuiLayer layer;
    bool shouldClose = false;
    bool inProject;
    std::string projPath;
    bool initializedDock;
    Console console;

    EditorState()
        : renderer(std::make_shared<Sauce::Renderer>()),
        scene(std::make_shared<Sauce::Scene>(renderer)),
        sceneSerializer(scene),
        layer(),
        shouldClose(false),
        inProject(false),
        projPath(""),
        initializedDock(false),
        console()
    {
    }
};


inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;
}

inline void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

inline void char_callback(GLFWwindow* window, unsigned int codepoint)
{
    ImGui_ImplGlfw_CharCallback(window, codepoint);
}

inline void enter_callback(GLFWwindow* window, int entered)
{
    ImGui_ImplGlfw_CursorEnterCallback(window, entered);
}

inline void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
}

inline void window_focus_callback(GLFWwindow* window, int focused)
{
    ImGui_ImplGlfw_WindowFocusCallback(window, focused);
}
