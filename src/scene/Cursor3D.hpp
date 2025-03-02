#pragma once

#include "Mesh.hpp"
#include "Object.hpp"
#include "Cursor3DObserver.hpp"
#include "MaterialContainer.hpp"


class Cursor3D : public Object3D, Cursor3DObserver
{
	Mesh mesh;
	MaterialContainer materials;
	DrawCommandGL cmd;

public:
	Cursor3D();

	virtual bool onCursor3D(const Cursor3DEvent& event) override;

	virtual void draw(DrawingQueue& drawing) override;
};

