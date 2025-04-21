#pragma once

#include "Vec.hpp"

class Image;
class Texture;
class DrawingQueue;

class OpenGLContext
{
public:
	static unsigned makeTexture(const Image *image);

	static void makeAndBindTexture(Texture& texture);

	static void deleteBuffer(unsigned bufferId);

	static int getContextId() { return contextId; }
	
	void contextChanged();

	void render(int width, int height, const Vec3D& eyePos, const DrawingQueue& drawingQueue);
	
private:
	static int maxTextureSize;
	static int contextId;
};
