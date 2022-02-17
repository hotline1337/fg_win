#include <Windows.h>
#include <string>
#include <random>
#include <intrin.h>
#include <iphlpapi.h>
#include <iostream>

std::string get_hwid()
{
    HW_PROFILE_INFOA hw; std::string ret;
    GetCurrentHwProfileA(&hw);

    const char* guid = hw.szHwProfileGuid;
	
    CHAR volumeName[MAX_PATH + 1] = { 0 };
    CHAR filesystemName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;

    GetVolumeInformationA("C:\\", volumeName, ARRAYSIZE(volumeName), &serialNumber, nullptr, nullptr, filesystemName, ARRAYSIZE(filesystemName));

    ret += guid;
    ret += "-";
    ret += std::to_string(serialNumber);
	
    return ret;
}

int __stdcall set_license()
{
    HKEY hkey;
    DWORD dwDisposition;
    DWORD value = 1;
    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\ActivationBroker\\fgp\\license", 0, nullptr, 0, KEY_WRITE, nullptr, &hkey, &dwDisposition) == ERROR_SUCCESS) 
    {
        RegSetValueExA(hkey, "value", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value));
    }
    return 0;
}

std::string random_string(std::string::size_type length)
{
    static auto& chrs = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{ std::random_device{}() };
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;
    s.reserve(length);

    while (length--)
        s += chrs[pick(rg)];
    return s;
}
