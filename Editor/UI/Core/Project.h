#pragma once

#include <fstream>
#include <string>

class Project
{
public:
	void Validate(const std::string &p)
	{
		path = p;
        if(path.ends_with("/"))
            path.resize(path.length() - 1);

        json j;
        std::ifstream f(std::format("{}/.project_info.json", path));
        f >> j;

        name = j["name"];
        currentScene = j["defaultScene"];
        if(!std::filesystem::exists(path + currentScene))
            std::runtime_error(std::format("path to shader: {}/{} is no valid path\n", path, currentScene).c_str());
        shaderPath = j["defaultShader"];
	}

	std::string path;
	std::string name;
	std::string currentScene;
	std::string shaderPath;
};
