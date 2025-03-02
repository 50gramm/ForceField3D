#pragma once

#include <bitset>
#include "Vec.hpp"
#include "DrawCommand.hpp"
#include "DrawingQueue.hpp"
#include "Mesh.hpp"


class SpaceFunc
{
public:
	real operator() (const Vec3D& r) const { return f(r); }

	virtual real f(const Vec3D& r) const = 0;
	virtual Vec3D df(const Vec3D& r) const = 0;
};


class SurfaceMeshing
{
	typedef VecT<int,3> GridPos;

	struct Cube
	{
		GridPos p0;
		real size;

		real values[8];
		int mask = 0;
	};

	const SpaceFunc* fx;

	real size;

	DynamicArray<Cube> cubes;

	std::bitset<256*256*256> visited;

	DrawCommandGL drawCmd;
	Mesh mesh;

	const Material* material = nullptr;

	int gridPosToId(const SurfaceMeshing::GridPos& p);

	GridPos getGridPosOfVert(const SurfaceMeshing::GridPos& p0, int vertIdx);

	GridPos toGridCoord(const Vec3D& r) const;
	
	Vec3D toWorldCoord(const GridPos& r) const;

	real value(const GridPos& r) const;

	Cube genCube(const GridPos& p);

	void traverse(const Cube& cube);

	Vec3D interpVertex(const Vec3D& p1, const Vec3D& p2, real v1, real v2);
	Vec3D interpVertex(const Cube& cube, int i, int j);

	void generateMesh(const Cube& cube);
	void generateMesh();

public:
	void setMaterial(const Material* material);

	void generate(const DynamicArray<Vec3D>& sources, const SpaceFunc& fv, real cubeSize);

	void draw(DrawingQueue& drawingQueue);
};
