#pragma once
#include <cmath>

#ifdef _DOUBLE_PRECISION
	using real = double;
	constexpr real EPS = 1E-10;
#else
	using real = float;
	constexpr real EPS = 1E-6f;
#endif // _DOUBLE_PRECISION

#ifndef M_PI
#	define M_PI 3.1415926535897932384626433832795
#endif

#define SQR(a) ((a)*(a))
