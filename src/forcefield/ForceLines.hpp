#pragma once

#include "Object.hpp"
#include "Mesh.hpp"
#include "KeyObserver.hpp"
#include "MaterialContainer.hpp"
#include "ForceFieldState.hpp"
#include "VisualSettings.hpp"


class ForceLines : public IVisual, public KeyObserver
{
	struct Gradient
	{
		Vec3D pos;
		Vec3D dir;
	};

	struct ForceLine
	{
		real startChargeValue = 0;
		real finishChargeValue = 0;
		DynamicArray<ForceLines::Gradient> points;
	};

	const ForceFieldState& state;
	const VisualSettings& visSettings;

	DynamicArray<ForceLine> lines;

	Mesh linesMesh;
	DynamicArray<real> tangential;
	DynamicArray<real> tangentialNormalized;
	DrawCommandGL drawCmd;
	ForceFieldState::UniqueId generatedStateId = 0;

	bool show = true;

	void generateLineMesh(const ForceLine& line);

	ForceLine generateLine(const PointCharge& source, const Vec3D dir, bool positiveDir) const;

	void regenerate();

public:
	ForceLines(const ForceFieldState& state, const VisualSettings& visSettings);

	virtual void draw(DrawingQueue& drawing) override;

	virtual bool onKey(const KeyEvent& event) override;
};
