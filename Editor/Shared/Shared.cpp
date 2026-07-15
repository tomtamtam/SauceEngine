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
