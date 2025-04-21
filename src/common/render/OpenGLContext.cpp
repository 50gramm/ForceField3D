#include "Error.hpp"
#include "OpenGLDefs.hpp"
#include "OpenGLContext.hpp"
#include "ShaderManager.hpp"
#include "DrawingQueuePlayback.hpp"


int OpenGLContext::contextId = -1;
int OpenGLContext::maxTextureSize = 0;


unsigned OpenGLContext::makeTexture(const Image *image)
{
	if(image->getPixels() == NULL)
		return -1;
	
	CHECK_GL_ERROR();

	GLuint texName;

	glGenTextures(1, &texName);

	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	ASSERT(image->getWidth() <= maxTextureSize && image->getHeight() <= maxTextureSize);

	const void* pixels = image->getPixels();

	if(image->getPixelFormat() == Image::PF_RGBA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	else if(image->getPixelFormat() == Image::PF_RGB)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	else
		ASSERT(0);

	CHECK_GL_ERROR();
	
	return texName;
}


void OpenGLContext::makeAndBindTexture(Texture& texture)
{
	if(texture.textureName == -1 || texture.contextId != contextId)
	{
		texture.textureName = makeTexture(texture.getImage());
		texture.contextId = contextId;
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture.textureName);
	}
}


void OpenGLContext::deleteBuffer(unsigned bufferId)
{
	glDeleteBuffers(1, &bufferId);
}


void OpenGLContext::contextChanged()
{
	if(contextId++ != -1)
		return;

	LOG_I(">>> OpenGL info >>>");
	LOG_I("  Version: %s", glGetString(GL_VERSION));
	LOG_I("  Vendor: %s", glGetString(GL_RENDERER));
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	LOG_I("  Max texture size: %d x %d", maxTextureSize, maxTextureSize);
	LOG_I("<<< OpenGL info <<<");
}


void OpenGLContext::render(int width, int height, const Vec3D& eyePos, const DrawingQueue& drawingQueue)
{
	glClearColor(0.f, 1.f, 0.f, 1.f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawingQueuePlayback playback;
	playback.playback(eyePos, width, height, drawingQueue);

	glFlush();
	glUseProgram(0);
	ShaderManager::frameEnd();
}
