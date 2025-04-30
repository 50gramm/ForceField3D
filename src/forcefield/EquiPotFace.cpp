#include "DrawingQueue.hpp"
#include "SurfaceMeshing.hpp"
#include "EquiPotFace.hpp"


struct PotFunc : public SpaceFunc
{
	const ForceFieldState& field;
	int sign = 1;

	PotFunc(const ForceFieldState& field)
		: field(field)
	{
		sign = (field.getChargeSum() < 0 ? -1 : 1);
	}

	virtual real f(const Vec3D& r) const override
	{
		return sign * field.getU(r);
	}

	virtual Vec3D df(const Vec3D& r) const override
	{
		return -sign * field.getE(r);
	}
};


EquiPotFace::EquiPotFace(const ForceFieldState& state, const VisualSettings& visSettings)
	: state(state)
	, visSettings(visSettings)
{
}


void EquiPotFace::regenerate()
{
	PotFunc potFunc(state);

	DynamicArray<Vec3D> sources;
	for(PointCharge charge : state.getCharges())
	{
		if((charge.charge < 0) == (potFunc.sign > 0))
			sources.add(charge.r);
	}

	SurfaceMeshing meshing(sources, potFunc, visSettings["EquiPotFace"]["VoxelSize"].get<real>());
	drawCmd = meshing.getMesh().genDrawCmd(visSettings.getMaterial("EquiPotFace"));

	generatedStateId = state.getId();
}


void EquiPotFace::draw(DrawingQueue& drawing)
{
	if(!show)
		return;
	
	if(generatedStateId != state.getId())
	{
		regenerate();
	}

	drawing.add(&drawCmd);
}


bool EquiPotFace::onKey(const KeyEvent& event)
{
	if(event.action == KeyEvent::ACTION_DOWN && event.keyCode == 'e')
	{
		show = !show;
		return true;
	}

	return false;
}