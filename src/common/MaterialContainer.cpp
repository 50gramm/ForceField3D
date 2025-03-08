#include <cstring>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <optional>
#include <filesystem>
#include "File.hpp"
#include "MaterialContainer.hpp"


MaterialContainer::MaterialContainer(const char* fileName)
{
	parseMaterials(fileName);
}


MaterialContainer::~MaterialContainer()
{
	for(auto& it : textures)
		delete it.second;
}


void MaterialContainer::parseMaterials(const char* fileName)
{
	FileContent content = File::read(fileName);
	std::stringstream ss(content.c_str());
	Material* material = nullptr;
	real alpha = 1.0;
	real r, g, b;

	char line[999];
	while(ss.getline(line, sizeof(line), '\n'))
	{
		std::stringstream sline(line);

		std::string token;
		sline >> token;

		sline.clear();
		sline.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
		try {
			if(token == "newmtl")
			{
				std::string matName;
				sline >> matName;

				ASSERT(!getMaterial(matName.c_str()));

				materials.emplace(matName, Material(matName));
				material = &materials.at(matName);
			}
			else if(token == "d")
			{
				sline >> alpha;
			}
			else if(token == "Ns")
			{
				sline >> material->specularExp;
				material->specularExp = std::max(real(0), std::min(material->specularExp, real(128)));
			}
			else if(token == "Ka")
			{
				sline >> r >> g >> b;
				material->ambient = {r, g, b, alpha};
			}
			else if(token == "Kd")
			{
				sline >> r >> g >> b;
				material->diffuse = {r, g, b, alpha};
			}
			else if(token == "Ks")
			{
				sline >> r >> g >> b;
				material->specular = {r, g, b, alpha};
			}
			else if(token == "map_Ka" || token == "map_Kd")
			{
				std::string textureName;
				sline >> textureName;
				if(!textures.count(textureName))
				{
					std::filesystem::path textureFullPath = std::filesystem::path(fileName).parent_path() / textureName;
					try {
						material->setTexture(textures[textureName] = new Texture(textureFullPath.string().c_str()));
					} catch(const std::string& str) {
						LOG_E("Failed to load rexture\n %s", str.c_str());
					}
				}
				else
				{
					material->setTexture(textures.at(textureName));
				}
			}
		} catch(const std::ios_base::failure& e) {
			ASSERT(0, "Parsing material from %s failed: %s", fileName, e.what());
			return;
		}
	}
}


const Material* MaterialContainer::getMaterial(const char* name) const
{
	if(!materials.count(name))
		return nullptr;
	return &materials.at(name);
}
