#pragma once

#include "ECS/Components.h"
#include <memory>
#ifdef _WIN32
  #include <windows.h>
  using DLLHandle = HMODULE;
#else
  #include <dlfcn.h>
  using DLLHandle = void*;
#endif

namespace Sauce
{
    using CreateScriptFn = std::shared_ptr<ScriptableEntity>(*)();
    using DeleteScriptFn = void(*)(ScriptableEntity*);

    struct ScriptHandle
    {
        DLLHandle Handle = nullptr;
        CreateScriptFn CreateScript = nullptr;
        DeleteScriptFn DeleteScript = nullptr;
    };

    class ScriptEngine
    {
    public:
        static ScriptHandle LoadScript(const std::string &name, const std::string &dllPath);
        static Script AttachScript(const ScriptHandle &h, const std::string &name);
    };
}