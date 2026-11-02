#include <filesystem>

#include "Core/ProjectSelector.h"

#include "Editor/UI/Core/Context.h"
#include "Editor/UI/Core/EditorApp.h"

void Edit(const std::string &path);

int main(int argc, char **argv)
{
    if(argc == 1)
    {
        ProjectSelector selector;
        std::string path = selector.Run();

        if(path == "exit")
            return 0;

        if(!std::filesystem::exists(std::format("{}/.project_info.json", path)))
        {
            std::runtime_error(std::format("path {} is no valid project path", path));
        }

        Edit(path);
    }
    else if(std::filesystem::exists(std::format("{}/.project_info.json", argv[1])))
    {
        Edit(argv[1]);
    }
}

void Edit(const std::string &path)
{
	EditorContext ctx{};
	EditorApp app{ctx};
	app.Run(path);
}
