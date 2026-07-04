#include "nlohmann/json_fwd.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define CREATE_ARG 0
#define RUN_ARG 1

int createProject(std::string name, std::string path);
void printArgError(int argc, char* argv[]);

int main(int argc, char *argv[])
{

    const std::unordered_map<int, std::string> firstArgWrapperI{{0,"create"},{1,"run"}};
    const std::unordered_map<std::string, int> firstArgWrapperS{{"create",0},{"run",1}};

    int firstArg = -1;
    for(int i = 0; i < argc - 1; i++)
    {
        std::string arg = argv[i + 1];

        if(i == 0)
        {
            firstArg = firstArgWrapperS.at(arg);
            if(firstArg == RUN_ARG && i == argc - 2)
            {
                std::cout << "run\n";
            }
        }
        else if(i == 1)
        {
            switch (firstArg)
            {
            case CREATE_ARG:
                if(i == argc - 2)
                    createProject(arg, ".");
                break;
            case RUN_ARG:
                printArgError(argc, argv);
                break;
            }
        }
        else if(i == 2)
        {
            switch (firstArg)
            {
            case CREATE_ARG:
                createProject(argv[2], argv[3]);
                break;
            case RUN_ARG:
                printArgError(argc, argv);
                return -1;
            }
        }
    }

    return 0;
}

void printArgError(int argc, char* argv[])
{
    std::string args;
    for(int i = 0; i < argc; i++)
    {
        args += ' ';
        if(i == 1) args += "\x1B[31m";
        args += argv[i];
    }
    args += "\033[0m\t\t";
    std::cerr << "Invalid set of arguments:" << args << '\n';
}

int createProject(std::string name, std::string path)
{
    if(!path.ends_with('/'))
        path += '/';

    std::string fp = path + name;
    std::filesystem::create_directory(fp);

    std::ofstream projInfo(fp + "/.project_info.json");

    json pj;
    pj["name"] = name;
    pj["defaultScene"] = "DefaultScene";

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

    defaultScene << sj;
    defaultScene.close();

    std::cout << "\x1B[32mcreated project: \033[0m\t\t" << "\x1B[34m" << path << name << "\033[0m\t\t" << '\n';
    return 0;
}
