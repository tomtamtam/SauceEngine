#include <cassert>
#include <fstream>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json ParseJsonFromFile(const std::string &path)
{
    std::ifstream FILE(path);
    if(!FILE)
    {
        std::cerr << "could not open file: " << path << '\n';
        assert(false);
    }

    std::string res;
    FILE >> res;
    json j = json::parse(res);
    return j;
}
