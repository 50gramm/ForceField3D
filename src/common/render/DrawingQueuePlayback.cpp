#include <set>
#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "IDrawingProgram.hpp"
#include "DrawingQueuePlayback.hpp"


// https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glFrustum.xml
static Matrix4 buildFrustum(real left, real right, real bottom, real top, real near, real far)
{
	Matrix4 mat(0);
	real A = (right + left) / (right - left);
	real B = (top + bottom) / (top - bottom);
	real C = -(far + near) / (far - near);
	real D = -2*far*near / (far - near);
	mat[0][0] = 2*near / (right - left);
	mat[1][1] = 2*near / (top - bottom);
	mat[0][2] = A;
	mat[1][2] = B;
	mat[2][2] = C;
	mat[2][3] = D;
	mat[3][2] = -1;
	mat[3][3] = 0;

	return mat;
}


static Matrix4 buildFrustum(const Vec3D& eyePos, real aspect, real near, real far)
{	
	// right = tan(fov/2) * nearZ
	// tan(fov/2) =  1 / eyePos.z

	real eyeZ = eyePos.z;
	real right =   near / eyeZ;
	real left =   -near / eyeZ;
	real top =     near / eyeZ * aspect;
	real bottom = -near / eyeZ * aspect;
	return buildFrustum(left, right, bottom, top, near, far);
}


void DrawingQueuePlayback::playback(const Vec3D& eyePos, int width, int height, const DrawingQueue& drawingQueue)
{
	globalState.eyePos = eyePos;
	globalState.projMatrix = buildFrustum(eyePos, real(height)/width, 0.1, 1000.0);

	std::set<DrawCommandPriority> priorities;
	for(const DrawingQueue::TransformedCmd& tcmd : drawingQueue.getCmd())
	{
		if(!tcmd.cmd->shaderName.empty())
			priorities.insert(tcmd.cmd->priority);
	}

	for(DrawCommandPriority priority : priorities)
	{
		for(const DrawingQueue::TransformedCmd& tcmd : drawingQueue.getCmd())
		{
			globalState.modelMatrix = tcmd.mat;

			if(!tcmd.cmd->shaderName.empty() && tcmd.cmd->priority == priority)
			{
				executeCommand(*tcmd.cmd);
			}
		}
	}
}


void DrawingQueuePlayback::executeCommand(const DrawCommand& cmd)
{
	assert(!cmd.shaderName.empty());
	IDrawingProgram* program = ShaderManager::getShader(cmd.shaderName);
	program->draw(globalState, cmd);

	Shader::unuseAll();
}
