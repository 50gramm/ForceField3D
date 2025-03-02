#pragma once

#include "DrawCommand.hpp"
#include "Shader.hpp"

class DefaultShader : public Shader
{
public:
	void load() override;

	void draw(const OpenGLState& globalState, const DrawCommand& cmd) override;

private:
	void setGlParams(const DrawCommandGL& cmd);

	void setLights();

	static const real t0;

	int uEyePosLoc = -1;
	int uProjectionMatrixLoc = -1;
	int uModelMatrixLoc = -1;
	int uNumLightsLoc = -1;
	int uTime = -1;
};
