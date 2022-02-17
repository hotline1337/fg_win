#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

#include "connect.h"
#include "misc.h"
#include "memory.h"
#include "hashing.h"

#pragma warning(disable : 6031)

using namespace std;

void __stdcall loadimage();

auto main(void) -> int
{
	class connect* connect = nullptr;

	LoadLibraryA("wininet.dll");
	LoadLibraryA("ole32.dll");
	LoadLibraryA("ws2_32.dll");
	LoadLibraryA("d3d9.dll");
	LoadLibraryA("oleaut32.dll");
	LoadLibraryA("ucrtbase.dll");

	const string hwid = md5(get_hwid()); 
	char license[32] = {0};
    SetConsoleTitleA("Fall Guys Premium");

	if (!connect->check_loader_version("28_08_1")) 
	{
		MessageBoxA(nullptr, "The loader is outdated. Download a new one via discord.", "FG_Premium Loader", MB_ICONERROR | MB_OK);
		TerminateProcess(GetCurrentProcess(), 0);
	}
	
	printf("Fall Guys Premium Loader | https://discord.gg/yxrzHq7 \n");
	if (connect->login(hwid))
	{
		printf("logged in succesfully\n");
		loadimage();
	}
	else
	{
		printf("enter license key: ");
		scanf("%32s", license);
		if (connect->license(license, hwid))
		{
			MessageBoxA(nullptr, "The license key has been redeemed successfully.", "FG_Premium Loader", MB_ICONINFORMATION | MB_OK);
			set_license();

			if (connect->check_cheat_version())
			{
				MessageBoxA(nullptr, "The cheat is currently outdated.", "FG_Premium Loader", MB_ICONERROR | MB_OK);
				TerminateProcess(GetCurrentProcess(), 0);
			}
			loadimage();
		}
		else
		{
			MessageBoxA(nullptr, "Failed to register the product.", "FG_Premium Loader", MB_ICONERROR | MB_OK);
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
	return EXIT_SUCCESS;
}

void __stdcall loadimage()
{
	class memory* memory = nullptr;
	class connect* connect = nullptr;

	printf("launching game...");
	system("start steam://rungameid/1097150");
	Sleep(2000);
	ShowWindow(GetConsoleWindow(), 0);

	while (memory->get_process_by_name("FallGuys_client.exe") == nullptr)
		Sleep(1000);

	Sleep(25000); /* smh */
	
	const string dll = connect->get_dll();
	std::vector<char> bytes(dll.begin(), dll.end());
	bytes.push_back('\0');
	char* buffer = &bytes[0];

	memory->manualmap_from_buffer(L"FallGuys_client.exe", buffer);
}