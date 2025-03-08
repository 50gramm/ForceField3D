#pragma once

#include "Object.hpp"
#include "ForceFieldState.hpp"
#include "VisualSettings.hpp"
#include "Cursor3D.hpp"
#include "Scene.hpp"
#include "PointCharges.hpp"
#include "ForceLines.hpp"
#include "EquiPotFace.hpp"
#include "SurfaceMeshing.hpp"
#include "SmartMovingOp.hpp"
#include "ChargeManipulator.hpp"

class Settings;


class ForceFieldOp : public Object3D
{
	ForceFieldState state;
	VisualSettings visSettings;
	
	Cursor3D cursor;
	Scene scene;

	Object3D spaceNode;

	PointCharges charges;
	ForceLines forceLines;
	EquiPotFace equiPotFace;

	ChargeManipulator chargeManipulator;
	SmartMovingOp smartMovingOp;

public:
	ForceFieldOp(const Settings& settings)
		: visSettings(settings["ForceField"]["Visual"])
		, scene(visSettings)
		, charges(state, visSettings)
		, forceLines(state, visSettings)
		, equiPotFace(state, visSettings)
		, chargeManipulator(state, spaceNode, charges)
		, smartMovingOp(spaceNode)
	{
		spaceNode.addChild(&charges);
		spaceNode.addChild(&forceLines);
		spaceNode.addChild(&equiPotFace);
		addChild(&cursor);
		addChild(&scene);
		addChild(&spaceNode);
	}
};
