#pragma once

#include "Array.hpp"
#include "DrawCommand.hpp"
#include "IDrawingProgram.hpp"


class LightHandler : public IDrawingProgram
{
	DynamicArray<LightSource> lights;

public:
	void draw(const OpenGLState& globalState, const DrawCommand& cmd) override;

	void frameEnd() override;

	const std::vector<LightSource>& getLights() const { return lights; }
};
