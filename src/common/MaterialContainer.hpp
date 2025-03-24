#pragma once

#include <map>
#include "Material.hpp"


class MaterialContainer
{
public:
	MaterialContainer() {}
	MaterialContainer(const char* fileName);
	~MaterialContainer();

	void parseMaterials(const char* fileName);

	const Material* getMaterial(const char* name) const;

private:
	std::map<std::string, Material> materials;
	std::map<std::string, std::shared_ptr<Texture>> textures;
};
