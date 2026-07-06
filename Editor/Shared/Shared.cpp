#include "Shared.h"

#include "nlohmann/json_fwd.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int createProject(const std::string &name, const std::string &path)
{
    std::string p = path;
    if(!p.ends_with('/'))
        p += '/';

    std::string fp = p + name;
    std::filesystem::create_directory(fp);

    std::ofstream projInfo(fp + "/.project_info.json");

    json pj;
    pj["name"] = name;
    pj["defaultScene"] = "Scenes/DefaultScene.json";
    pj["defaultShader"] = "Shaders/Default.glsl";

    projInfo << pj;
    projInfo.close();

    std::filesystem::create_directory(fp + "/Code");
    std::filesystem::create_directory(fp + "/Scenes");
    std::filesystem::create_directory(fp + "/Shaders");

    std::filesystem::copy_file("/usr/share/sauce-engine/presets/Main.cpp", fp + "/Code/Main.cpp");
    std::filesystem::copy_file("/usr/share/sauce-engine/presets/Default.glsl", fp + "/Shaders/Default.glsl");
    std::filesystem::copy_file("/usr/share/sauce-engine/presets/CMakeLists.txt", fp + "/CMakeLists.txt");

    std::ofstream defaultScene(fp + "/Scenes/DefaultScene.json");

    json sj;
    sj["name"] = "TestScene";
    sj["id"] = randUUID();
    sj["objects"] = json::array();

    defaultScene << sj;
    defaultScene.close();

    std::cout << "\x1B[32mcreated project: \033[0m" << "\x1B[34m" << p<< name << "\033[0m" << '\n';
    return 0;
}

uint64_t randUUID()
{
    return s_UniformDistribution(s_Editor);
}

Editor::Editor(const std::string &path)
{
    std::ifstream projinfo(path + ".project_info.json");
    if(!projinfo)
    {
        std::cerr << "\x1B[31mERROR:\033[0m" << " failed to load project at " << path << " (no .project_info.json)\n";
        return;
    }

    json jprojinfo;
    projinfo >> jprojinfo;

    m_ProjectName = jprojinfo["name"];
    m_DefaultScene = jprojinfo["defaultScene"];
    m_DefaultShader = jprojinfo["defaultShader"];

    Scene s = LoadScene(m_DefaultScene);
}

Component getComponentById(int id)
{
    switch (id)
    {
    case TRANSFORM_CIDX:
    {
        transform_component c;
        return c;
    }
    case SCRIPT_CIDX:
    {
        script_component c;
        return c;
    }
    case CAMERA_CIDX:
    {
        camera_component c;
        return c;
    }
    case MESH_INSTANCE_CIDX:
    {
        meshinstance_component c;
        return c;
    }
    }
    Component c;
    return c;
}

Scene Editor::LoadScene(const std::string &path)
{
    std::ifstream f(path);
    if(!f)
    {
        std::cerr << "\x1B[31mERROR:\033[0m failed to find Scene at " << path << "\n";
    }

    json j;
    f >> j;

    Scene s;
    s.Name = j["name"];
    s.UUID = j["id"];
    if(j["objects"].size() == 0)
        return s;

    for(int i = 0; i < j["objects"].size(); i ++)
    {
        json jo = j["objects"][i];
        Object o;
        o.Name = jo["name"];
        o.UUID = jo["id"];

        if(jo["components"].size() == 0)
        {
            s.Objects.at(o.UUID) = o;            continue;
            continue;
        }

        for(int j = 0; j < jo["components"].size(); i++)
        {
            json jc = jo["components"][j];
            int id = jc["id"];

            o.Components.at(o.UUID) = getComponentById(id);
        }
    }

    return s;
}

Editor::~Editor()
{}

std::string Editor::GetProjectName() const
{
    return m_ProjectName;
}


std::string Editor::GetDefaultScene() const
{
    return m_DefaultScene;
}

std::string Editor::GetDefaultShader() const
{
    return m_DefaultShader;
}
