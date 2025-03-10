#pragma once
#include "Settings.hpp"
#include "MaterialContainer.hpp"


class VisualSettings
{
	const Json& jVis;
	MaterialContainer materials;

public:
	VisualSettings(const Json& settings)
		: jVis(settings)
	{
		materials.parseMaterials(jVis["Materials"].get<std::string>().c_str());
	}

	const Json& operator[](const char* key) const
	{
		return jVis[key];
	}

	const Material* getMaterial(const char* name) const
	{
		return materials.getMaterial(name);
	}
};
