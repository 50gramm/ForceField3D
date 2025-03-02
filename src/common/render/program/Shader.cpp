#include <cstdlib>
#include "Error.hpp"
#include "StringUtils.hpp"
#include "OpenGLDefs.hpp"
#include "OpenGLContext.hpp"
#include "Shader.hpp"


static GLuint
loadShader(GLenum type, const char *shaderSrc)
{
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

	if(!compiled)
	{
		char infoLog[4096];
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		glDeleteShader(shader);

		throw sformat("GLSL>> Error compiling shader:\n%s\n", infoLog);
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
		throw sformat("GLSL>> glCreateProgram failed: %d", glGetError());
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

		throw sformat("GLSL>> Failed to link program:\n%s\n", infoLog);
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
