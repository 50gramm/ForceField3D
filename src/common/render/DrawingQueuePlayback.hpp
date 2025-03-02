#pragma once

#include "DrawingQueue.hpp"


class DrawingQueuePlayback
{
	OpenGLState globalState;

	void executeCommand(const DrawCommand& cmd);

public:
	void playback(const Vec3D& eyePos, int width, int height, const DrawingQueue& queue);
};
