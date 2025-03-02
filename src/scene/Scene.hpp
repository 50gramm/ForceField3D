#pragma once
#include "Object.hpp"
#include "MeshObject.hpp"
#include "VisualSettings.hpp"


class Scene : public IVisual
{
	DrawCommandLight lightCmd;
	MeshObject skyBox;

public:
	Scene(const VisualSettings& visSettings);

	virtual void draw(DrawingQueue& drawing) override;
};
