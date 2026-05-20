#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <sstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json ParseJsonFromFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "could not open file: " << path << '\n';
        assert(false);
    }

    json j;
    file >> j;
    return j;
}
