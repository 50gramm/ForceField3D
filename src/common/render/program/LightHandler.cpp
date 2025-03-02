#include "Error.hpp"
#include "ShaderManager.hpp"
#include "LightHandler.hpp"


void LightHandler::draw(const OpenGLState& /*globalState*/, const DrawCommand& baseCmd)
{
	const DrawCommandLight& cmd = *dynamic_cast<const DrawCommandLight*>(&baseCmd);

	lights.push_back(cmd.light);
}


void LightHandler::frameEnd()
{
	lights.clear();
}


static DrawingProgramRegistrator<LightHandler> staticProgramRegistrator("LightHandler");
