#pragma once

#include <string>

struct OpenGLState;
struct DrawCommand;
class ShaderManager;

class IDrawingProgram
{
	friend class ShaderManager;

public:
	virtual ~IDrawingProgram();

	virtual void draw(const OpenGLState& globalState, const DrawCommand& cmd) = 0;

	virtual void frameEnd() {};
};
