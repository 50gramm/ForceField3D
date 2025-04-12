#include "Error.hpp"
#include "Timing.hpp"
#include "StringUtils.hpp"
#include "OpenGLDefs.hpp"
#include "ShaderManager.hpp"
#include "LightHandler.hpp"
#include "DefaultShader.hpp"


void DefaultShader::load()
{
	uEyePosLoc = glGetUniformLocation(id, "uEyePos");
	uModelMatrixLoc = glGetUniformLocation(id, "uModelMatrix");
	uProjectionMatrixLoc = glGetUniformLocation(id, "uProjectionMatrix");
	uNumLightsLoc = glGetUniformLocation(id, "uNumLights");

	uTime = glGetUniformLocation(id, "uTime");

	CHECK_GL_ERROR();
}


void DefaultShader::draw(const OpenGLState& globalState, const DrawCommand& baseCmd)
{
	const DrawCommandGL& cmd = *dynamic_cast<const DrawCommandGL*>(&baseCmd);

	use();

	setGlParams(cmd);

	setLights();

	transmitter.setUniform(uProjectionMatrixLoc, globalState.projMatrix);
	transmitter.setUniform(uModelMatrixLoc, globalState.modelMatrix);
	transmitter.setUniform(uEyePosLoc, globalState.eyePos);
	transmitter.setUniform(uTime, (real)Timing::getSec());

	transmitter.setVariables(cmd.vars);

	if(cmd.indexBegin < cmd.indexEnd)
	{
		if(!cmd.indices.empty())
		{
			if(!cmd.indexBufferId)
			{
				glGenBuffers(1, &cmd.indexBufferId);
				ASSERT(cmd.indexBufferId);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmd.indexBufferId);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, (cmd.indexEnd - cmd.indexBegin)*sizeof(cmd.indices[0]), &cmd.indices[cmd.indexBegin], GL_STATIC_DRAW);
			}
			else
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmd.indexBufferId);
			}
			
			glDrawElements((int)cmd.primitive, cmd.indexEnd - cmd.indexBegin, GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays((int)cmd.primitive, cmd.indexBegin, cmd.indexEnd);
		}
		CHECK_GL_ERROR();
	}

	transmitter.disableAttribs();
}


void DefaultShader::setGlParams(const DrawCommandGL& cmd)
{
	glEnable(GL_DEPTH_TEST);

	if(cmd.flags & int(GLFlags::DisableCullFace))
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}

	if(cmd.priority == DrawCommandPriority::Transparent)
	{
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}

	CHECK_GL_ERROR();
}


void DefaultShader::setLights()
{
	LightHandler* lightHandler = dynamic_cast<LightHandler*>(ShaderManager::getShader("LightHandler"));
	ASSERT(lightHandler);

	int idx = 0;
	for(const LightSource& light : lightHandler->getLights())
	{
		std::string lightName = "uPointLights[" + std::to_string(idx) + "]";

		transmitter.setUniform(lightName + ".position", light.pos4);

		transmitter.setUniform(lightName + ".ambient", light.material.ambient);
		transmitter.setUniform(lightName + ".diffuse", light.material.diffuse);
		transmitter.setUniform(lightName + ".specular", light.material.specular);

		transmitter.setUniform(lightName + ".constant", light.constant);
		transmitter.setUniform(lightName + ".linear", light.linear);
		transmitter.setUniform(lightName + ".quadratic", light.quadratic);

		++idx;
	}

	transmitter.setUniform(uNumLightsLoc, idx);
}


const real DefaultShader::t0 = Timing::getSec();

static DrawingProgramRegistrator<DefaultShader> staticProgramRegistrator("default");
