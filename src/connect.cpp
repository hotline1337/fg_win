#include <windows.h>
#include <string>
#include <WinInet.h>
#include <iostream>

#include "connect.h"

#pragma comment(lib,"WinInet.lib")
#pragma comment(lib,"advapi32.lib")

using namespace std;

const string link[5] = {
	"http://35.205.13.241/fg_loader/login.php?hwid=%s", // <= LOGIN
	"http://35.205.13.241/fg_loader/license.php?license=%s&hwid=%s", // <= REGISTER
	"http://35.205.13.241/fg_loader/loader.txt", // <= LOADER VERSION
	"http://35.205.13.241/fg_loader/z_nv.txt", // <= CHEAT VERSION
	"http://35.205.13.241/fg_loader/kRGsdAMMvEyb.txt" // <= DLL
};

string replace_all(string subject, const string& search, const string& replace) 
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) 
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}
string download_string(const string url) 
{
	const HINTERNET connection = InternetOpenA("list_access\r\n", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, NULL);
	string rtn;
	if (connection) 
	{
		const HINTERNET url_file = InternetOpenUrlA(connection, url.c_str(), nullptr, NULL, NULL, NULL);
		if (url_file) 
		{
			char buffer[2000];
			DWORD bytes_read;
			do 
			{
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);
			InternetCloseHandle(connection);
			InternetCloseHandle(url_file);
			string p = replace_all(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(connection);
	string p = replace_all(rtn, "|n", "\r\n");
	return p;
}

int connect::login(std::string hwid_string) 
{
	char request[512];
	sprintf(request, link[0].c_str(), hwid_string.c_str());
	
	string result = download_string(request);
	if (result == "logged")
	{
		return 1;
	}
	return 0;
}

int connect::license(std::string license_key, std::string hwid_string) 
{
	char request[512];
	sprintf(request, link[1].c_str(), license_key.c_str(), hwid_string.c_str());

	string result = download_string(request);
	if (result == "registered")
	{
		return 1;
	}
	return 0;
}

int connect::check_loader_version(std::string version)
{
	string result = download_string(link[2]);
	if (result == version)
	{
		return 1;
	}
	return 0;
}

int connect::check_cheat_version()
{
	string result = download_string(link[3]);
	if (result == "outdated")
	{
		return 1;
	}
	return 0;
}

string connect::get_dll()
{
	return download_string(link[4]);
}
