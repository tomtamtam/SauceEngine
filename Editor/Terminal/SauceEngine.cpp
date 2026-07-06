#include "../Shared/Shared.h"

#include "nlohmann/json_fwd.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

#define CREATE_ARG 0
#define RUN_ARG 1
#define EDIT_ARG 2

void printArgError(int argc, char* argv[]);
void edit(const std::string &path);

int main(int argc, char *argv[])
{

    const std::unordered_map<int, std::string> firstArgWrapperI{{0,"create"},{1,"run"}, {2, "edit"}};
    const std::unordered_map<std::string, int> firstArgWrapperS{{"create",0},{"run",1}, {"edit", 2}};

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
            if(firstArg == EDIT_ARG && i == argc - 2)
            {
                edit("./");
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
            case EDIT_ARG:
                if(i == argc - 2)
                {
                    if(!arg.ends_with('/'))
                        arg.push_back('/');
                    edit(arg);
                }
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
            case EDIT_ARG:
                printArgError(argc, argv);
                break;
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

// Eddit
void printHelp()
{
    std::cout << "Help:\nh = help?\nl $any = list smth (l h to see all options)\n";
}

const std::unordered_map<std::string, std::string> aliases =
{
    {"help", "h"},
    {"list", "l"}
};

const std::unordered_map<std::string, std::string> helpMap =
{
    {"h", "Help of Help?! (:"},
    {"l", "l $arg\nname: Shows project name\n"}
};

void list(const std::string second)
{
    if(aliases.contains(second))
    {
        std::cout << helpMap.at(aliases.at(second)) << '\n';
    }
    else
    {
        std::cout << helpMap.at(second) << '\n';
    }
}

void help(const std::string &second)
{
    if(second == "")
    {
        printHelp();
        return;
    }

    if(aliases.contains(second))
    {
        std::cout << helpMap.at(aliases.at(second)) << '\n';
    }
    else
    {
        std::cout << helpMap.at(second) << '\n';
    }
}

const std::unordered_map<std::string, std::function<void(const std::string &second)>> cmndMap = 
{
    {"h", [](const std::string &second){printHelp();}},
    {"l", [](const std::string &second){list(second);}}
};

void getOutput(const std::string &first, const std::string &second)
{
    if(aliases.contains(first))
    {
        cmndMap.at(aliases.at(first))(second);
    }
    else
    {
        cmndMap.at(first)(second);
    }
}

void processInput(std::string line)
{
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token)
    {
        tokens.push_back(token);
    }

    if(tokens.size() == 1)
        getOutput(tokens.at(0), "");
    else if(tokens.size() == 2)
        getOutput(tokens.at(0), tokens.at(1));
}

void edit(const std::string &path)
{
    auto editor = std::make_shared<Editor>(path);

    std::system("clear");

    std::cout << "Welcome to SauceEngine's commandline editor!\n" << "\x1B[32mEditing\033[0m " << editor->GetProjectName() << '\n';

    bool isEditing = true;

    while(isEditing)
    {
        std::string cmnd;
        std::cout << "> ";
        std::getline(std::cin, cmnd);
        processInput(cmnd);
    }
}
