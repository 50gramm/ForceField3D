#include "ForceFieldOp.hpp"
#include "MeshGeneration.hpp"


PointCharges::PointCharges(const ForceFieldState& state, const VisualSettings& visSettings)
	: state(state)
{
	chargeMesh = GenSphere(1., 24, 48);
	chargesMat[1][ChargeOverlay::OverlayNormal] = visSettings.materials.getMaterial("PositiveCharge");
	chargesMat[0][ChargeOverlay::OverlayNormal] = visSettings.materials.getMaterial("NegativeCharge");
	chargesMat[1][ChargeOverlay::OverlaySelected] = visSettings.materials.getMaterial("SelectedPositiveCharge");
	chargesMat[0][ChargeOverlay::OverlaySelected] = visSettings.materials.getMaterial("SelectedNegativeCharge");
	chargesMat[1][ChargeOverlay::OverlayPicked] = visSettings.materials.getMaterial("PickedPositiveCharge");
	chargesMat[0][ChargeOverlay::OverlayPicked] = visSettings.materials.getMaterial("PickedNegativeCharge");
}


void PointCharges::draw(DrawingQueue& drawing)
{
	DrawCommandGL defCmd = chargeMesh.genDrawCmd();

	cmds.clear();
	cmds.reserve(state.charges.size());
	for(const PointCharge& charge : state.charges)
	{
		cmds.emplace_back(defCmd);
		DrawCommandGL& cmd = cmds.back();

		cmd.vars.addVariable("uMaterial", *chargesMat[charge.charge < 0. ? 0 : 1][overlays[charge.getId()]]);

		Matrix4 pos({}, charge.r, getRadius(charge.charge));
		drawing.pushMatrix(pos);
		drawing.add(&cmd);
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
	return sqrt(fabs(charge)) * 0.05;
}
