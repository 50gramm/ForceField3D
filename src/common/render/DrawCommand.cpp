#include "OpenGLContext.hpp"
#include "DrawCommand.hpp"


DrawCommand::~DrawCommand()
{
}


DrawCommandGL& DrawCommandGL::operator=(DrawCommandGL&& other)
{
	if(indexBufferId)
		OpenGLContext::deleteBuffer(indexBufferId);

	shaderName = std::move(other.shaderName);
	priority = other.priority;

	primitive = other.primitive;
	flags = other.flags;

	indices = other.indices;
	indexBegin = other.indexBegin;
	indexEnd = other.indexEnd;
	indexBufferId = other.indexBufferId;

	vars = std::move(other.vars);

	return *this;
}


DrawCommandGL::~DrawCommandGL()
{
	if(indexBufferId)
		OpenGLContext::deleteBuffer(indexBufferId);
}
