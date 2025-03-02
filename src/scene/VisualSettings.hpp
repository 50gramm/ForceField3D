#pragma once
#include "MaterialContainer.hpp"


struct VisualSettings
{
	MaterialContainer materials;

	VisualSettings(const char* materialName)
	{
		materials.parseMaterials(materialName);
	}
};
