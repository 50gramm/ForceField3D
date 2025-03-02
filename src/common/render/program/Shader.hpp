#pragma once

#include <string>
#include "IDrawingProgram.hpp"
#include "ShaderVariable.hpp"

struct OpenGLState;
struct DrawCommand;
class ShaderManager;


class Shader : public IDrawingProgram
{
	friend class ShaderManager;

protected:
	GLSLVariableTransmitter transmitter;

public:
	unsigned int id = 0;
	int contextId = -1;

	Shader();
	virtual ~Shader();

	virtual void load() = 0;

	virtual bool isLoaded() const;
	
	void use() const;

	static void unuseAll();

	unsigned getUniformLocation(const char* name) const;

protected:
	void load(const char* vertexShader, const char* fragmentShader);
};
