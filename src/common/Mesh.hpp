#pragma once

#include "Array.hpp"
#include "Vec.hpp"
#include "Material.hpp"
#include "DrawCommand.hpp"


struct Mesh
{
	DynamicArray<Vec3D> verts;
	DynamicArray<Vec3D> norms;
	DynamicArray<Vec2D> texCoords;
	DynamicArray<int> tris;


	void fillDrawCmd(DrawCommandGL& cmd, const Material* material=nullptr) const
	{
		cmd.primitive = GLPrimitives::Triangles;
		cmd.vars.addVariable<std::vector<Vec3D>>("aPosition", verts);
		cmd.vars.addVariable<std::vector<Vec2D>>("aTexCoord", texCoords);
		cmd.vars.addVariable<std::vector<Vec3D>>("aNormal", norms);
		cmd.indices = tris;
		cmd.indexEnd = (tris.empty() ? verts.size() : tris.size());
		if(material != nullptr)
			cmd.vars.addVariable("uMaterial", *material);
		cmd.priority = ((material && material->isTransparent()) ? DrawCommandPriority::Transparent : DrawCommandPriority::Default);
	}


	DrawCommandGL genDrawCmd(const Material* material=nullptr) const
	{
		DrawCommandGL cmd;
		cmd.shaderName = "default";
		fillDrawCmd(cmd, material);
		return cmd;
	}
};
