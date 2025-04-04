#include <cstring>
#include <map>
#include "Platform.h"
#include "File.hpp"



static std::map<std::string, std::string> folderMap;


void File::setFolder(const std::string& folder, const std::string& name)
{
	folderMap[folder] = name;
}


void File::write(const char *fileName, const void* buff, int size)
{
	std::string fullPath = fileName;
	for(const auto& it : folderMap)
	{
		if(!strncmp(fileName, it.first.c_str(), it.first.size()))
			fullPath = it.second + (fileName + it.first.size());
	}

	platform_write_file(fullPath.c_str(), buff, size);
}


FileContent File::read(const char *fileName)
{
	std::string fullPath = fileName;
	for(const auto& it : folderMap)
	{
		if(!strncmp(fileName, it.first.c_str(), it.first.size()))
			fullPath = it.second + (fileName + it.first.size());
	}

	void* buff = nullptr;
	size_t size = platform_read_file(fullPath.c_str(), &buff);
	
	FileContent content(size, buff);

	return content;
}