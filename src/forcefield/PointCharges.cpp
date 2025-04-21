#include "ForceFieldOp.hpp"
#include "MeshGeneration.hpp"


PointCharges::PointCharges(const ForceFieldState& state, const VisualSettings& visSettings)
	: state(state)
{
	chargeMesh = GenSphere(1., 24, 48);
	chargesMat[1][ChargeOverlay::OverlayNormal] = visSettings.getMaterial("PositiveCharge");
	chargesMat[0][ChargeOverlay::OverlayNormal] = visSettings.getMaterial("NegativeCharge");
	chargesMat[1][ChargeOverlay::OverlaySelected] = visSettings.getMaterial("SelectedPositiveCharge");
	chargesMat[0][ChargeOverlay::OverlaySelected] = visSettings.getMaterial("SelectedNegativeCharge");
	chargesMat[1][ChargeOverlay::OverlayPicked] = visSettings.getMaterial("PickedPositiveCharge");
	chargesMat[0][ChargeOverlay::OverlayPicked] = visSettings.getMaterial("PickedNegativeCharge");
	for(int i=0; i<2; ++i)
	{
		for(int j=0; j<ChargeOverlay::OverlayEnd; ++j)
		{
			cmds[i][j] = chargeMesh.genDrawCmd();
			cmds[i][j].vars.addVariable("uMaterial", *chargesMat[i][j]);
		}
	}
}


void PointCharges::draw(DrawingQueue& drawing)
{
	for(const PointCharge& charge : state.getCharges())
	{
		Matrix4 pos({}, charge.r, getRadius(charge.charge));
		drawing.pushMatrix(pos);
		drawing.add(&cmds[charge.charge < 0. ? 0 : 1][overlays[charge.getId()]]);
		drawing.popMatrix();
	}
}


void PointCharges::setOverlay(PointCharge::UniqueId id, PointCharges::ChargeOverlay overlay)
{
	overlays[id] = overlay;
}


PointCharges::ChargeOverlay PointCharges::getOverlay(PointCharge::UniqueId id) const
{
	if(!overlays.count(id))
		return ChargeOverlay::OverlayNormal;
	return overlays.at(id);
}


real PointCharges::getRadius(real charge)
{
	return pow(fabs(charge), 1.0/3.0) * 0.05;
}
