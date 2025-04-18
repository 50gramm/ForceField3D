#pragma once

#ifdef ANDROID
#	include <GLES3/gl3.h>
#	include <GLES2/gl2.h>
#	include <GLES/gl.h>
#elif EMSCRIPTEN
#	define GL_GLEXT_PROTOTYPES
#	include <GLES2/gl2.h>
#else
#	include <glad/glad.h>
#	include <GL/gl.h>
#endif
#include "Error.hpp"


#ifdef _DOUBLE_PRECISION
#	define GL_REAL			GL_DOUBLE
#else
#	define GL_REAL			GL_FLOAT
#endif


#ifdef _DEVELOPMENT
#	define CHECK_GL_ERROR() {											\
		for (GLint error = glGetError(); error; error = glGetError()) { \
			LOG_E("glError = 0x%x in %s() at %s:%d", error, __FUNCTION__, __FILE__, __LINE__);\
			DBG_BREAK \
		}																 \
	}
#else
#	define CHECK_GL_ERROR()
#endif
