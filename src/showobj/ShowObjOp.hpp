#pragma once

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
	ShowObjOp()
		: visSettings("ff.mtl")
		, scene(visSettings)
		, obj("tmp/test_obj/planet.obj")
		, smartMovingOp(spaceNode)
	{
		obj.setPosition(Matrix4({Vec3D(1,0,0),M_PI/2}, Vec3D(0,0,0), 0.1));
		spaceNode.addChild(&obj);
		addChild(&cursor);
		addChild(&scene);
		addChild(&spaceNode);
	}
};
