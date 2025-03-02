#pragma once

#include "Object.hpp"
#include "ForceFieldState.hpp"
#include "PointCharges.hpp"
#include "Cursor3DObserver.hpp"
#include "KeyObserver.hpp"


class ChargeManipulator : public Cursor3DObserver, KeyObserver
{
	ForceFieldState& state;
	const Object3D& parentNode;
	PointCharges& chargesVis;
	PointCharge::UniqueId pickedChargeId = -1;
	PointCharge::UniqueId selectedChargeId = -1;
	Vec3D prevCursorPos;

	PointCharge::UniqueId getChargeIdAtPoint(const Vec3D& r) const;

public:
	ChargeManipulator(ForceFieldState& state, const Object3D& chargesParentNode, PointCharges& chargesVis);

	virtual bool onCursor3D(const Cursor3DEvent& event) override;
	
	virtual bool onKey(const KeyEvent& event) override;
};
