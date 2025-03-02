#pragma once

#include <map>
#include "Array.hpp"
#include "Vec.hpp"
#include "MaterialContainer.hpp"
#include "Object.hpp"
#include "DrawingQueue.hpp"
#include "Mesh.hpp"


class MeshObject : public Object3D
{
public:
	MeshObject(const char* filePath);

	void draw(DrawingQueue& drawingQueue) override;

	void setShader(const std::string& name);
	
private:
	void parseVertex(const char* line);
	void parseNormal(const char* line);
	void parseTexCoord(const char* line);
	void parseFace(const char* line);

	void generateGlCommands();

	const char* skipSpaces(const char* pc);

	struct Face
	{
		DynamicArray<int> vertexIndices;
		DynamicArray<int> normalIndices;
		DynamicArray<int> texCoordIndices;
		const Material* material;
	};

	DynamicArray<Vec3D> verts;
	DynamicArray<Vec3D> norms;
	DynamicArray<Vec2D> texCoords;

	std::map<const Material*, Mesh> meshesByMat;

	DynamicArray<Face> faces;

	MaterialContainer materials;
	const Material* currMaterial = nullptr;
	DynamicArray<DrawCommandGL> cmds;
};
