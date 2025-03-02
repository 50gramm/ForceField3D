#pragma once

#include <string>
#include "Vec.hpp"
#include "TransformationMatrix.hpp"
#include "Material.hpp"
#include "ShaderVariable.hpp"

class Material;


enum class DrawCommandPriority
{
	Light,
	Default,
	Transparent
};


enum class GLPrimitives
{
	Points        = 0x0000,
	Lines         = 0x0001,
	LineLoop      = 0x0002,
	LineStrip     = 0x0003,
	Triangles     = 0x0004,
	TriangleStrip = 0x0005,
	TriangleFan   = 0x0006
};


enum class GLFlags
{
	DisableCullFace = 0x01
};


struct DrawCommand
{
	virtual ~DrawCommand() {}

	std::string shaderName;
	DrawCommandPriority priority = DrawCommandPriority::Default;
};


struct DrawCommandGL : public DrawCommand
{
	GLPrimitives primitive = GLPrimitives::Triangles;
	unsigned flags = 0;

	DynamicArray<int> indices;
	int indexBegin = 0, indexEnd = 0;

	ShaderVariableContainer vars;
};


struct LightSource
{
	Material material;
	Vec4D pos4;
	real constant = 0;
	real linear = 0;
	real quadratic = 1;
};


struct DrawCommandLight : public DrawCommand
{
	DrawCommandLight() { priority = DrawCommandPriority::Light; }
	
	LightSource light;
};


struct OpenGLState
{
	Matrix4 projMatrix;
	Matrix4 modelMatrix;

	Vec3D eyePos;
};
