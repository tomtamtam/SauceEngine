#include <dlfcn.h>
#include <print>

#include <iostream>

#include "Core/ScriptEngine.h"


namespace Sauce
{
    ScriptHandle ScriptEngine::LoadScript(const std::string &name, const std::string &dllPath)
    {
		std::println("LoadScript name: {} path: {}", name, dllPath);
        ScriptHandle dll;
        #ifdef _WIN32
            dll.Handle = LoadLibraryA(dllPath.c_str());
            if(!dll.Handle) return ScriptDLL{};
            std::string createName = "Create" + name;
            std::string deleteName = "Delete" + name;

            dll.CreateScript = (CreateScriptFn) GetProcAddress(dll.Handle, createName);
            dll.CreateScript = (SeleteScriptFn) GetProcAddress(dll.Handle, deleteName);
        #else
            dll.Handle = dlopen(dllPath.c_str(), RTLD_LAZY);
            if(!dll.Handle)
            {
                std::cout << "handle failed " << dlerror() << '\n';
                return ScriptHandle{};
            }

            std::string createName = "Create" + name;
            std::string deleteName = "Delete" + name;

            dll.CreateScript = (CreateScriptFn) dlsym(dll.Handle, createName.c_str());
            dll.DeleteScript = (DeleteScriptFn) dlsym(dll.Handle, deleteName.c_str());
        #endif

        if(!dll.CreateScript || !dll.DeleteScript) 
        {
            std::cout << "create / delete failed " << dlerror() << '\n';
            return ScriptHandle{};
        }
        return dll;
    }

    Script ScriptEngine::AttachScript(const ScriptHandle &h, const std::string &name, const std::string &path)
    {
		std::println("AttachScript name: {} path: {}", name, path);
        Script comp;
        comp.ScriptName = name;
        comp.ScriptPath = path;
        comp.InitializeScript = h.CreateScript;
        comp.DestroyScript = h.DeleteScript;
        comp.Instance = comp.InitializeScript();
        return comp;
    }
}
