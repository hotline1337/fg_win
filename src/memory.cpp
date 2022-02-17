#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <TlHelp32.h>

#include "memory.h"

#include "../BlackBone/src/BlackBone/Process/Process.h"
#pragma comment(lib, "../BlackBone/build/x64/Release/BlackBone.lib")

using namespace std;

int memory::get_process_id_by_name(const std::string& p_name)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 structprocsnapshot = { 0 };

    structprocsnapshot.dwSize = sizeof(PROCESSENTRY32);

    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;
        
    if (Process32First(snapshot, &structprocsnapshot) == FALSE)return 0;

    while (Process32Next(snapshot, &structprocsnapshot))
    {
        if (!strcmp(structprocsnapshot.szExeFile, p_name.c_str()))
        {
            CloseHandle(snapshot);
            return structprocsnapshot.th32ProcessID;
        }
    }
    CloseHandle(snapshot);
    return 0;

}

HANDLE memory::get_process_by_name(PCSTR name)
{
    DWORD pid = 0;
    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    if (Process32First(snapshot, &process))
    {
        do
        {
            if (string(process.szExeFile) == string(name))
            {
                pid = process.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    if (pid != 0)
        return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    return nullptr; // Not found
}

void __cdecl memory::manualmap_from_buffer(const wchar_t* process_name, char* buffer)
{
    blackbone::Process process;
    if (process.Attach(process_name) != STATUS_SUCCESS) 
    {
        ShowWindow(GetConsoleWindow(), 0);
        MessageBoxA(nullptr, "Cannot load cheat [1]", "Error", MB_ICONERROR | MB_OK);
        exit(0);
    }

    auto mod_found = false;
    while (!mod_found) 
    {
        for (auto [fst, snd] : process.modules().GetAllModules()) 
        {
            if (fst.first == L"kernel32.dll")
                mod_found = true;
        }
        Sleep(250);
    }

    const auto size = sizeof(buffer) / sizeof(buffer[0]);
    static bool driver_control = blackbone::Driver().EnsureLoaded(L"map.sys");
    if (!driver_control)
    {
        const auto rs = blackbone::Driver().MmapDll(process.pid(), buffer, size, false, KWipeHeader);
        if (!rs.success()) 
        {
            ShowWindow(GetConsoleWindow(), 0);
            MessageBoxA(nullptr, ("Cannot load cheat [" + to_string(rs.status) + "]").c_str(), "Error", MB_ICONERROR | MB_OK);
            exit(0);
        }
    }
    else
    {
        ShowWindow(GetConsoleWindow(), 0);
        MessageBoxA(nullptr, "Driver is not loaded", "Error", MB_ICONERROR | MB_OK);
        exit(0);
    }
    process.Detach();
}