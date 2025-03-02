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
			glDrawElements((int)cmd.primitive, cmd.indexEnd - cmd.indexBegin, GL_UNSIGNED_INT, &cmd.indices[cmd.indexBegin]);
		else
			glDrawArrays((int)cmd.primitive, cmd.indexBegin, cmd.indexEnd);
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
	if(lightHandler == nullptr)
		throw std::runtime_error("LightHandler not found");

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
