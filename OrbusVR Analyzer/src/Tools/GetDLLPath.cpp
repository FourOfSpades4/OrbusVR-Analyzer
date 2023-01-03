#include "GetDLLPath.h"

#include <iostream>
#include <Windows.h>
#include <Shlwapi.h>
#include <filesystem>

static std::string s_PathToDll;
static std::string s_DllParentDirectory;

namespace DllPath {
    const std::string& GetPath()
    {
        return s_PathToDll;
    }

    const std::string& GetParent()
    {
        return s_DllParentDirectory;
    }

    bool Initialize()
    {
        HMODULE hm = NULL;
        
        // Get handle to this module using an address within it
        if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&Initialize, &hm) == 0) {
            return false;
        }

        char path[MAX_PATH];
        // Use that module handle to get the path to the DLL
        if (GetModuleFileNameA(hm, path, sizeof(path)) == 0) {
            return false;
        }

        s_PathToDll = std::string(path);
        s_DllParentDirectory = s_PathToDll.substr(0, s_PathToDll.find_last_of("/\\"));

        return true;
    }

    std::string MakeRelative(std::string_view path)
    {
        return (std::filesystem::path{ s_DllParentDirectory } / std::filesystem::path{ path }).string();
    }
}

