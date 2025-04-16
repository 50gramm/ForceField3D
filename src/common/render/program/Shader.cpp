#include <cstdlib>
#include "Error.hpp"
#include "Exception.hpp"
#include "StringUtils.hpp"
#include "OpenGLDefs.hpp"
#include "OpenGLContext.hpp"
#include "Shader.hpp"


static GLuint
loadShader(GLenum type, const char *shaderSrc)
{
#ifdef GL_ES_VERSION_2_0
	std::string shaderSrcStr = "#version 100\n";
#else
	std::string shaderSrcStr = "#version 120\n";
#endif
	shaderSrcStr += shaderSrc;
	shaderSrc = shaderSrcStr.c_str();

	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if(shader == 0)
		return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	char infoLog[4096];
	glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
	if(infoLog[0])
	{
		LOG_E("GLSL>> Shader compile log:\n%s", infoLog);
	}

	if(!compiled)
	{
		glDeleteShader(shader);

		throw Exception("GLSL>> Failed to compile shader");
	}

	return shader;
}


Shader::Shader()
{}


Shader::~Shader()
{}


void Shader::load(const char* vShader, const char* fShader)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = loadShader(GL_VERTEX_SHADER, vShader);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShader);

	// Create the program object
	id = glCreateProgram();

	if(id == 0)
	{
		throw Exception("GLSL>> glCreateProgram failed: %u", glGetError());
	}

	transmitter = GLSLVariableTransmitter(id);

	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);

	// Link the program
	glLinkProgram(id);

	// Check the link status
	glGetProgramiv(id, GL_LINK_STATUS, &linked);

	if(!linked) 
	{
		char infoLog[4096];
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		glDeleteProgram(id);

		throw Exception("GLSL>> Failed to link program:\n%s\n", infoLog);
	}

	contextId = OpenGLContext::getContextId();

	load();
}


bool Shader::isLoaded() const
{
	return id != 0 && contextId == OpenGLContext::getContextId();
}


void Shader::use() const
{
	glUseProgram(id);
}


void Shader::unuseAll()
{
	glUseProgram(0);
}


unsigned Shader::getUniformLocation(const char* name) const
{
	return glGetUniformLocation(id, name);
}
