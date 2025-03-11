#include "File.hpp"
#include "JsonUtils.hpp"
#include "TransformationMatrix.hpp"
#include "ForceFieldState.hpp"


PointCharge::UniqueId PointCharge::nextId = 1;
ForceFieldState::UniqueId ForceFieldState::nextStateId = 1;


void ForceFieldState::changed()
{
	stateId += 1;
	nextStateId += 1;
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
	changed();
}


void ForceFieldState::addCharge(const PointCharge& charge)
{
	ASSERT(!idToIdx.count(charge.getId()));
	idToIdx[charge.getId()] = charges.size();
	charges.push_back(charge);
	changed();
}


void ForceFieldState::delCharge(UniqueId id)
{
	charges[idToIdx[id]] = charges.back();
	idToIdx[charges.back().getId()] = idToIdx[id];
	charges.pop_back();
	idToIdx.erase(id);
	changed();
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


Json ForceFieldState::dumpToJson() const
{
	Json jRes;
	for(const PointCharge& charge : getCharges())
	{
		Json jCharge;
		jCharge["Pos"] = {charge.r.x, charge.r.y, charge.r.z};
		jCharge["Charge"] = charge.charge;
		jRes["Charges"].push_back(jCharge);
	}

	return jRes;
}


ForceFieldState ForceFieldState::parseJson(const Json& jState)
{
	ForceFieldState state;

	Json jCharges = jState["Charges"];
	for(const auto& jCharge : jCharges)
	{
		state.addCharge(PointCharge(
			Vec3D(
				jCharge["Pos"][0],
				jCharge["Pos"][1],
				jCharge["Pos"][2]
			),
			jCharge["Charge"]
		));
	}

	return state;
}
