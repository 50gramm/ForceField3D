#pragma once
#include <nlohmann/json.hpp>
#include "Error.hpp"


class Settings
{
	nlohmann::json jSettings;

public:
	using json = nlohmann::json;

	Settings(const char* fileName);

	const json& operator[](const char* key) const;
};
