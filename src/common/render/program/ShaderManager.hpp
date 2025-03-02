#pragma once

#include <string>
#include "IDrawingProgram.hpp"

class ShaderManager
{
public:
	static void registerProgram(const std::string& name, IDrawingProgram* shader);
	
	static IDrawingProgram* getShader(const std::string& name);

	static void frameEnd();
};


template<class ProgramType, class... Args>
struct DrawingProgramRegistrator
{
	DrawingProgramRegistrator(const char* name, Args... args)
	{
		IDrawingProgram *shader = new ProgramType(args...);
		ShaderManager::registerProgram(name, shader);
	}
};
