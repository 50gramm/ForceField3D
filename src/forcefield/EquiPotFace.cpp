#include "ForceFieldOp.hpp"


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
	surfaceMeshing.setMaterial(visSettings.getMaterial("EquiPotFace"));
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

	surfaceMeshing.generate(sources, potFunc, visSettings["EquiPotFace"]["VoxelSize"].get<real>());

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

	surfaceMeshing.draw(drawing);
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