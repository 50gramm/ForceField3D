#include "File.hpp"
#include "Settings.hpp"


Settings::Settings(const char* filePath)
{
	jSettings = parseJsonFile(filePath);
}


const Json& Settings::operator[](const char* key) const
{
	return jSettings[key];
}
