#pragma once

class memory
{
public:
	static void __cdecl manualmap_from_buffer(const wchar_t* process_name, char* buffer);
	static HANDLE get_process_by_name(PCSTR name);
	static int get_process_id_by_name(const std::string& p_name);
};
