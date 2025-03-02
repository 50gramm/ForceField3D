#pragma once

#include <cmath>
#include <cstdlib>
#include "Vec.hpp"


template<class T>
T clamp(T val, T min, T max)
{
	return std::min(std::max(min, val), max);
}


inline bool isNear(real a, real b)
{
	return fabs(a - b) < EPS;
}


inline bool isNear(const Vec3D &v1, const Vec3D &v2)
{
	return fabs(v1.x - v2.x) < EPS && fabs(v1.y - v2.y) < EPS;
}


inline bool isNearZero(const Vec3D &v)
{
	return v.lenSqr() < (EPS*EPS);
}


inline real frand(void)
{
	return real(rand() * 1.0 / (RAND_MAX-1));
}
