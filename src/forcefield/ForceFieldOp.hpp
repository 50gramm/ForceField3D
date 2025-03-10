#pragma once

#include "Json.hpp"
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


class ForceFieldOp : public Object3D, public KeyObserver
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
	ForceFieldOp(const Settings& settings);

	virtual bool onKey(const KeyEvent& event) override;

	Json dumpToJson() const;
	void parseJson(const Json& jState);
};
