#pragma once

class connect
{
public:
	static int login(std::string hwid_string);
	static int license(std::string license_key, std::string hwid_string);
	static int check_loader_version(std::string version);
	static int check_cheat_version();
	static std::string get_dll();
};
