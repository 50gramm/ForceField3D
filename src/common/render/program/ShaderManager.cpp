#include <map>
#include "Error.hpp"
#include "File.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"


static std::map<std::string, IDrawingProgram*>& getShaderStore()
{
	static std::map<std::string, IDrawingProgram*> shaderStore;

	return shaderStore;
}


void ShaderManager::registerProgram(const std::string& name, IDrawingProgram* shader)
{
	getShaderStore()[name] = shader;
	LOG_I("Drawing program '%s' registered.", name.c_str());
}


IDrawingProgram* ShaderManager::getShader(const std::string& name)
{
	if(!getShaderStore().count(name))
		throw std::string("Shader not registered: ") + name;

	IDrawingProgram* program = getShaderStore()[name];
	Shader* shader = dynamic_cast<Shader*>(program);

	if(shader == nullptr || shader->isLoaded())
		return program;
	
	LOG_I("GLSL>> Creating shader '%s'...", name.c_str());

	std::string vsFileName = std::string("{assets}/shaders/") + name + ".vs";
	FileContent vsShaderSrc = File::read(vsFileName.c_str());

	std::string fsFileName = std::string("{assets}/shaders/") + name + ".fs";
	FileContent fsShaderSrc = File::read(fsFileName.c_str());

	shader->load(vsShaderSrc.c_str(), fsShaderSrc.c_str());

	LOG_I("GLSL>> Shader '%s' created", name.c_str());

	return shader;
}


void ShaderManager::frameEnd()
{
	for(const auto& it : getShaderStore())
	{
		it.second->frameEnd();
	}
}
