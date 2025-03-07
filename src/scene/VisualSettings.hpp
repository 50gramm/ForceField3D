#pragma once
#include "Settings.hpp"
#include "MaterialContainer.hpp"


class VisualSettings
{
	const Settings::json& jVis;
	MaterialContainer materials;

public:
	VisualSettings(const Settings::json& settings)
		: jVis(settings)
	{
		materials.parseMaterials(jVis["materials"].get<std::string>().c_str());
	}

	const Settings::json& operator[](const char* key) const
	{
		return jVis[key];
	}

	const Material* getMaterial(const char* name) const
	{
		return materials.getMaterial(name);
	}
};
