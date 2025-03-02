#include <cstdio>
#include <optional>
#include <exception>
#include "Core.hpp"
#include "Platform.h"
#include "Error.hpp"
#include "StringUtils.hpp"
#include "File.hpp"



static std::string folders[File::SPEC_FOLDER_NUM];


void File::set_folder(SpecialFolder folder, const std::string& name)
{
	folders[folder] = name;
}

/*
bool File::write(const char *file_name, const void* buff, int size)
{
	std::string full_path = (!folders[USER].empty() ? (folders[USER] + "/") : std::string("")) + file_name;

	return platform_write_file(full_path.c_str(), buff, size) != 0;
}
*/

FileContent File::read(const char *file_name, SpecialFolder folder)
{
	std::string full_path = (!folders[folder].empty() ? (folders[folder] + "/") : std::string("")) + file_name;

	void* buff = nullptr;
	size_t size = platform_read_asset(full_path.c_str(), &buff);
	
	FileContent content(size, buff);

	return content;
}