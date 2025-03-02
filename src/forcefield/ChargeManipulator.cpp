#include "ChargeManipulator.hpp"


ChargeManipulator::ChargeManipulator(ForceFieldState& state, const Object3D& chargesParentNode, PointCharges& chargesVis)
	: state(state)
	, parentNode(chargesParentNode)
	, chargesVis(chargesVis)
{}


PointCharge::UniqueId ChargeManipulator::getChargeIdAtPoint(const Vec3D& r) const
{
	for(const PointCharge& charge : state.getCharges())
	{
		if(charge.r.distSqr(r) < SQR(PointCharges::getRadius(charge.charge)))
		{
			return charge.getId();
		}
	}

	return PointCharge::invalidId;
}


bool ChargeManipulator::onCursor3D(const Cursor3DEvent& event)
{
	if(event.button == 0 && event.type == Cursor3DEvent::EVENT_DOWN)
	{
		pickedChargeId = getChargeIdAtPoint(parentNode.getAbsMat().inverse() * event.pos);
		chargesVis.setOverlay(pickedChargeId, PointCharges::OverlayPicked);
	}
	else if(event.button == 0 && event.type == Cursor3DEvent::EVENT_MOVE)
	{
		chargesVis.setOverlay(selectedChargeId, PointCharges::OverlayNormal);		
		selectedChargeId = PointCharge::invalidId;

		const PointCharge* pickedCharge = state.getCharge(pickedChargeId);
		if(pickedCharge)
		{
			PointCharge pickedChargeCopy = *pickedCharge;
			pickedChargeCopy.r += (parentNode.getAbsMat().inverse() * event.pos - parentNode.getAbsMat().inverse() * prevCursorPos);
			state.setCharge(pickedChargeCopy);
			selectedChargeId = pickedChargeId;
		}
		else
		{
			selectedChargeId = getChargeIdAtPoint(parentNode.getAbsMat().inverse() * event.pos);
			pickedChargeId = PointCharge::invalidId;
		}

		if(pickedChargeId != PointCharge::invalidId)
		{
			chargesVis.setOverlay(pickedChargeId, PointCharges::OverlayPicked);
		}
		else if(selectedChargeId != PointCharge::invalidId)
		{
			chargesVis.setOverlay(selectedChargeId, PointCharges::OverlaySelected);
		}
	}
	else
	{
		chargesVis.setOverlay(pickedChargeId, PointCharges::OverlayNormal);
		pickedChargeId = PointCharge::invalidId;
	}

	prevCursorPos = event.pos;

	return (pickedChargeId != PointCharge::invalidId);
}


bool ChargeManipulator::onKey(const KeyEvent& event)
{
	if(event.action == KeyEvent::ACTION_DOWN && (event.keyCode == 'p' || event.keyCode == 'n'))
	{
		const PointCharge* selectedCharge = state.getCharge(selectedChargeId);
		if(selectedCharge)
		{
			PointCharge selectedChargeCopy = *selectedCharge;
			selectedChargeCopy.charge += (event.keyCode == 'p' ? 1.0 : -1.0);
			state.setCharge(selectedChargeCopy);
			if(std::abs(selectedCharge->charge) < EPS)
			{
				state.delCharge(selectedChargeId);
				selectedChargeId = getChargeIdAtPoint(parentNode.getAbsMat().inverse() * prevCursorPos);
			}
			return true;
		}
		else
		{
			PointCharge charge(parentNode.getAbsMat().inverse() * prevCursorPos, (event.keyCode == 'p' ? 1.0 : -1.0));
			state.addCharge(charge);
			selectedChargeId = getChargeIdAtPoint(parentNode.getAbsMat().inverse() * prevCursorPos);
			return true;
		}
	}

	return false;
}
