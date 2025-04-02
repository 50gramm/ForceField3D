#pragma once

#include "Object.hpp"
#include "Mesh.hpp"
#include "MaterialContainer.hpp"
#include "ForceFieldState.hpp"
#include "VisualSettings.hpp"


class PointCharges : public IVisual
{
public:
	enum ChargeOverlay
	{
		OverlayNormal,
		OverlaySelected,
		OverlayPicked,
		OverlayEnd
	};

private:
	const ForceFieldState& state;
	const Material* chargesMat[2][OverlayEnd] = {};

	Mesh chargeMesh;
	DynamicArray<DrawCommandGL> cmds;

	std::map<PointCharge::UniqueId, ChargeOverlay> overlays;

public:
	PointCharges(const ForceFieldState& state, const VisualSettings& visSettings);

	virtual void draw(DrawingQueue& drawing) override;

	void setOverlay(PointCharge::UniqueId id, ChargeOverlay overlay);
	ChargeOverlay getOverlay(PointCharge::UniqueId id) const;

	static real getRadius(real charge);
};
