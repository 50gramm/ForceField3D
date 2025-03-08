#include "File.hpp"
#include "Settings.hpp"


Settings::Settings(const char* fileName)
{
	FileContent content = File::read(fileName, File::ASSET);

	jSettings = nlohmann::json::parse(content.c_str(), nullptr, true, true);
}


const Settings::json& Settings::operator[](const char* key) const
{
	return jSettings[key];
}
