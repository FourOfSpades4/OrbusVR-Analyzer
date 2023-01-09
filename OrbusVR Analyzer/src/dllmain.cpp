#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <thread>
#include "framework.h"

#include "GetDLLPath.h"
#include "AnalyzerApp.h"

#include <WinSock2.h>
#include <wx/msw/winundef.h>
#include <wx/wxprec.h>
#include <wx/wx.h>

DWORD WINAPI Main(LPVOID lpParameter);

bool OnDllProcessAttach(HMODULE module) {
    CreateThread(NULL, 0, Main, NULL, 0, NULL);

    return true;
}

bool OnDllThreadAttach(HMODULE module) {
    return true;
}

bool OnDllThreadDetach(HMODULE module) {
    return true;
}

bool OnDllProcessDetach(HMODULE module) {
    wxEntryCleanup();
    return true;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        return OnDllProcessAttach(hModule);
    case DLL_THREAD_ATTACH:
        return OnDllThreadAttach(hModule);
    case DLL_THREAD_DETACH:
        return OnDllThreadDetach(hModule);
    case DLL_PROCESS_DETACH:
        return OnDllProcessDetach(hModule);
    }
    return TRUE;
}

DWORD WINAPI Main(LPVOID lpParameter) {
    AllocConsole();
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    FILE* fout;
    FILE* fin;
    freopen_s(&fout, "CONOUT$", "w", stdout);
    freopen_s(&fin, "CONIN$", "r", stdin);

    std::cout << "Successfully injected Analyzer\n";

    if (DllPath::Initialize() == false) {
        std::cout << "Failed getting path to DLL. Aborting\n";
        return -1;
    }


    std::cout << "Path to DLL is \"" << DllPath::GetPath() << "\"" << std::endl;

    wxApp::SetInstance(new AnalyzerApp());
    wxEntry(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);

    return true;
}