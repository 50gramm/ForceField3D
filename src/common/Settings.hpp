#pragma once
#include "JsonUtils.hpp"
#include "Error.hpp"


class Settings
{
	Json jSettings;

public:
	Settings(const char* filePath);

	const Json& operator[](const char* key) const;
};
