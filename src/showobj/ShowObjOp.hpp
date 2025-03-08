#pragma once

#include "Settings.hpp"
#include "Object.hpp"
#include "MeshObject.hpp"
#include "Cursor3D.hpp"
#include "Scene.hpp"
#include "SmartMovingOp.hpp"
#include "VisualSettings.hpp"


class ShowObjOp : public Object3D
{
	VisualSettings visSettings;
	Cursor3D cursor;
	Scene scene;
	MeshObject obj;
	Object3D spaceNode;
	SmartMovingOp smartMovingOp;

public:
	ShowObjOp(const Settings& settings)
		: visSettings(settings["ShowObj"]["Visual"])
		, scene(visSettings)
		, obj(settings["ShowObj"]["Obj"].get<std::string>().c_str())
		, smartMovingOp(spaceNode)
	{
		obj.setPosition(Matrix4({Vec3D(1,0,0),M_PI/2}, Vec3D(0,0,0), 0.1));
		spaceNode.addChild(&obj);
		addChild(&cursor);
		addChild(&scene);
		addChild(&spaceNode);
	}
};
