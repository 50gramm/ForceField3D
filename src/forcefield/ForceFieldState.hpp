
#pragma once
#include <map>
#include "Array.hpp"
#include "Vec.hpp"


class PointCharge
{
public:
	typedef long long UniqueId;
	static constexpr UniqueId invalidId = -1;

private:
	static UniqueId nextId;
	UniqueId id = nextId++;

public:
	Vec3D r;
	real charge;

	PointCharge(const Vec3D& r, real charge)
		: r(r)
		, charge(charge)
	{}

	UniqueId getId() const { return id; }

	bool isPositive() const { return 0 < charge; }
	bool isNegative() const { return 0 > charge; }
};


class ForceFieldState
{
public:
	typedef long long UniqueId;

private:
	UniqueId stateId = 1;

	DynamicArray<PointCharge> charges;
	std::map<PointCharge::UniqueId, int> idToIdx;

public:
	ForceFieldState();

	UniqueId getId() const { return stateId; }

	const DynamicArray<PointCharge>& getCharges() const { return charges; }

	const PointCharge* getCharge(PointCharge::UniqueId id) const;

	void setCharge(const PointCharge& charge);
	void addCharge(const PointCharge& charge);
	void delCharge(UniqueId id);

	real getChargeSum() const;

	real getU(const Vec3D& r) const;
	Vec3D getE(const Vec3D& r) const;
};
