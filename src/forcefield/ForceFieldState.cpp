
#include "TransformationMatrix.hpp"
#include "ForceFieldState.hpp"

PointCharge::UniqueId PointCharge::nextId = 1;


ForceFieldState::ForceFieldState()
{
	// TODO: Read from file
	Matrix4 baseRot(Rotation3DData(Vec3D(0,0,1), real(M_PI/3)), Vec3D(0,0,0));
	Vec3D baseR1(0.0, 0.3*sqrt(3)/2, 0.0);
	Vec3D baseR2 = baseR1 * 1.9;
	Matrix4 rot(1);
	for(int i=0; i<3; ++i)
	{
		addCharge({rot * baseR1, -1.0});
		rot *= baseRot;
		if(i == 2)
			addCharge({rot * baseR2 + Vec3D(0.27, -0.1, 0.0), 1.2});
		else
			addCharge({rot * baseR2, 1.2});
		rot *= baseRot;
	}
}


const PointCharge* ForceFieldState::getCharge(PointCharge::UniqueId id) const
{
	if(!idToIdx.count(id))
		return nullptr;
	return &charges[idToIdx.at(id)];
}


void ForceFieldState::setCharge(const PointCharge& charge)
{
	ASSERT(idToIdx.count(charge.getId()));
	charges[idToIdx.at(charge.getId())] = charge;
	stateId += 1;
}


void ForceFieldState::addCharge(const PointCharge& charge)
{
	ASSERT(!idToIdx.count(charge.getId()));
	idToIdx[charge.getId()] = charges.size();
	charges.push_back(charge);
	stateId += 1;
}


void ForceFieldState::delCharge(UniqueId id)
{
	charges[idToIdx[id]] = charges.back();
	idToIdx[charges.back().getId()] = idToIdx[id];
	charges.pop_back();
	idToIdx.erase(id);
	stateId += 1;
}


real ForceFieldState::getChargeSum() const
{
	real q = 0.0;
	for(const PointCharge& charge : charges)
		q += charge.charge;
	return q;
}


real ForceFieldState::getU(const Vec3D& r) const
{
	real U = 0.0;
	for(const PointCharge& charge : charges)
	{
		real dist = (r - charge.r).len();
		U += charge.charge / dist;
	}
	return U;
}


Vec3D ForceFieldState::getE(const Vec3D& r) const
{
	Vec3D E(0,0,0);
	for(const PointCharge& charge : charges)
	{
		Vec3D dr = r - charge.r;
		real dist = dr.len();
		E += dr * (charge.charge / (dist*dist*dist));
	}
	return E;
}
