#pragma once

#include "Object.hpp"
#include "Mesh.hpp"
#include "MaterialContainer.hpp"
#include "ForceFieldState.hpp"
#include "VisualSettings.hpp"


class ForceLines : public IVisual
{
	struct Gradient
	{
		Gradient(const Vec3D& pos, const Vec3D& dir)
			: pos(pos)
			, dir(dir)
		{}

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

	void generateLineMesh(const ForceLine& line);

	ForceLine generateLine(const PointCharge& source, const Vec3D dir, bool positiveDir) const;

	void regenerate();

public:
	ForceLines(const ForceFieldState& state, const VisualSettings& visSettings);
	virtual void draw(DrawingQueue& drawing) override;
};
