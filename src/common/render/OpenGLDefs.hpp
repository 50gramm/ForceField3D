#pragma once

#ifdef ANDROID
#	include <GLES3/gl3.h>
#	include <GLES2/gl2.h>
#	include <GLES/gl.h>
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
			LOG_E("after %s():%d glError (0x%x)\n", __FUNCTION__, __LINE__, error);\
			DBG_BREAK \
		}																 \
	}
#else
#	define CHECK_GL_ERROR()
#endif
