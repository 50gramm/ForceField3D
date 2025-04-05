#pragma once

#include <assert.h>
#include <cmath>
#include <iostream>
#include "Core.hpp"
#include "Error.hpp"


#if 0
template<class T>
struct Pos2T
{
	Pos2T() {}
	Pos2T(T x, T y)
		: x(x)
		, y(y)
	{}

	bool operator== (const Pos2T &b) const
	{
		return x == b.x && y == b.y;
	}

	bool operator!= (const Pos2T &b) const
	{
		return x != b.x || y != b.y;
	}

	Pos2T operator* (T q) const
	{
		return Pos2T(x*q,y*q);
	}
	
	Pos2T operator+ (const Pos2T &b) const
	{
		return Pos2T(x+b.x,y+b.y);
	}
	
	Pos2T &operator+= (const Pos2T &b)
	{
		x += b.x;
		y += b.y;
		return *this;
	}
	
	Pos2T operator- (const Pos2T &b) const
	{
		return Pos2T(x-b.x,y-b.y);
	}
	
	Pos2T &operator-= (const Pos2T &b)
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}
	
	Pos2T operator- () const
	{
		return Pos2T(-x,-y);
	}
	
	Pos2T &operator*= (T q)
	{
		x *= q;
		y *= q;
		return *this;
	}
	
	T operator* (const Pos2T &b) const
	{
		return x*b.x + y*b.y;
	}
	
	T operator% (const Pos2T &b) const
	{
		return x*b.y - y*b.x;
	}

	const Pos2T &pos() const
	{
		return *this;
	}
	
	double len() const 
	{
		return sqrt(x*x + y*y);
	}
	
	T lenSqr() const 
	{
		return x*x + y*y;
	}
	
	Pos2T norm() const
	{
		return (*this)*(1.0/len());
	}

	Pos2T rot90() const
	{
		return Pos2T(-y, x);
	}
	
	double dist(const Pos2T &p) const
	{
		return sqrt((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y));
	}
	
	T dist2(const Pos2T &p) const
	{
		return (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y);
	}	
	
	bool operator< (const Pos2T &b) const
	{
		if(y != b.y)
			return y < b.y;
		return x < b.x;
	}

	T &operator[] (int i)
	{
		assert(i == 0 || i == 1);
		return (i == 0 ? x : y);
	}

	const T &operator[] (int i) const
	{
		assert(i == 0 || i == 1);
		return (i == 0 ? x : y);
	}

	T x, y;
};


template<class T>
struct Pos3T
{
	Pos3T() {}
	Pos3T(T x, T y, T z)
		: x(x)
		, y(y)
		, z(z)
	{}

	Pos3T(const Pos2T<T> &p, T z)
		: x(p.x)
		, y(p.y)
		, z(z)
	{}

	void setZero()
	{
		x = y = z = 0;
	}

	bool operator== (const Pos3T &b) const
	{
		return x==b.x && y==b.y && z==b.z;
	}
	
	bool operator!= (const Pos3T &b) const
	{
		return x!=b.x || y!=b.y || z!=b.z;
	}
	
	Pos3T operator+= (const Pos3T &b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		return *this;
	}
	
	Pos3T operator+ (const Pos3T &b) const
	{
		return Pos3T(x+b.x, y+b.y, z+b.z);
	}
	
	Pos3T operator-= (const Pos3T &b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return *this;
	}
	
	Pos3T operator- (const Pos3T &b) const
	{
		return Pos3T(x-b.x, y-b.y, z-b.z);
	}

	Pos3T operator- () const
	{
		return Pos3T(-x, -y, -z);
	}
	
	Pos3T& operator*= (T q)
	{
		x *= q;
		y *= q;
		z *= q;
		return *this;
	}
	
	Pos3T operator* (T q) const
	{
		return Pos3T(x*q, y*q, z*q);
	}

	friend Pos3T operator* (T q, const Pos3T &b)
	{
		return Pos3T(b.x*q, b.y*q, b.z*q);
	}

	Pos3T &operator/= (T q)
	{
		x /= q;
		y /= q;
		z /= q;
		return *this;
	}
	
	Pos3T operator/ (T q) const
	{
		return Pos3T(x/q, y/q, z/q);
	}

	T operator* (const Pos3T &b) const
	{
		return x*b.x + y*b.y + z*b.z;
	}
	
	Pos3T operator% (const Pos3T &b) const
	{
		return Pos3T(
			y*b.z - z*b.y,
			z*b.x - x*b.z,
			x*b.y - y*b.x
		);
	}

	Pos3T mul(const Pos3T &b) const
	{
		return Pos3T(x*b.x, y*b.y, z*b.z);
	}
	
	double len() const
	{
		return sqrt(x*x + y*y + z*z);
	}
	
	T lenSqr() const
	{
		return x*x + y*y + z*z;
	}
	
	Pos3T norm() const
	{
		return (*this) * (1.0/len());
	}

	void normalize()
	{
		(*this) *= (1.0/len());
	}
	
	double dist(const Pos3T &b) const
	{
		return sqrt((x-b.x)*(x-b.x) + (y-b.y)*(y-b.y) + (z-b.z)*(z-b.z));
	}
	
	T distSqr(const Pos3T &b) const
	{
		return (x-b.x)*(x-b.x) + (y-b.y)*(y-b.y) + (z-b.z)*(z-b.z);
	}
	
	bool operator< (const Pos3T &b) const
	{
		if(z != b.z)
			return z < b.z;
		if(y != b.y)
			return y < b.y;
		return x < b.x;
	}

	T &operator[] (int i)
	{
		assert(0 <= i && i < 3);
		return (&x)[i];
	}

	const T &operator[] (int i) const
	{
		assert(0 <= i && i < 3);
		return (&x)[i];
	}

	T x, y, z;
};


typedef Pos3T<real> Vec3D;


template<class T>
std::ostream& operator<<(std::ostream &s,const Pos2T<T> &p)
{
	s << "(" << p.x << "," << p.y << ")";
	return s;
}

template<class T>
std::ostream& operator<<(std::ostream &s,const Pos3T<T> &p)
{
	s << "(" << p.x << "," << p.y << "," << p.z << ")";
	return s;
}


#elif 0


template<class T, int N>
struct VecBaseT
{
protected:
	T elems[N];
};


template<class T, int N>
struct VecSpecT;


template<class T, int N>
struct VecSpecT : public VecBaseT<T,N>
{
};


template<class T>
struct VecSpecT<T,3> : public VecBaseT<T,3>
{
	using VecBaseT<T,3>::elems;

	T& x = elems[0];
	T& y = elems[1];
	T& z = elems[2];
};


template<class T, int N>
struct VecT : public VecSpecT<T,N>
{
	using VecBaseT<T,N>::elems;

	VecT()
	{}

	VecT(const VecSpecT<T,N>& b)
	{
		for(int i=0; i<N; ++i)
			elems[i] = b.elems[i];
	}

	template<typename... Args>
	VecT(Args... args)
	{
		init(0, args...);
	}

	void init(int index, T a1)
	{
		elems[index] = a1;
	}

	template<typename... Args>
	void init(int index, T a1, Args... args)
	{
		elems[index] = a1;
		init(index+1, args...);
	}

	VecT& operator= (const VecT& b)
	{
		for(int i=0; i<N; ++i)
			elems[i] = b.elems[i];
		return *this;
	}

	void setZero()
	{
		for(int i=0; i<N; ++i)
			elems[i] = 0;
	}

	bool operator== (const VecT &b) const
	{
		for(int i=0; i<N; ++i)
			if(elems[i] != b.elems[i])
				return false;
		return true;
	}
	
	bool operator!= (const VecT &b) const
	{
		for(int i=0; i<N; ++i)
			if(elems[i] != b.elems[i])
				return true;
		return false;
	}
	
	VecT operator+= (const VecT &b)
	{
		for(int i=0; i<N; ++i)
			elems[i] += b.elems[i];
		return *this;
	}
	
	VecT operator+ (const VecT &b) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.elems[i] = elems[i] + b.elems[i];
		return res;
	}
	
	VecT operator-= (const VecT &b)
	{
		for(int i=0; i<N; ++i)
			elems[i] -= b.elems[i];
		return *this;
	}
	
	VecT operator- (const VecT &b) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.elems[i] = elems[i] - b.elems[i];
		return res;
	}

	VecT operator- () const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.elems[i] = -elems[i];
		return res;
	}
	
	VecT& operator*= (T q)
	{
		for(int i=0; i<N; ++i)
			elems[i] *= q;
		return *this;
	}
	
	VecT operator* (T q) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.elems[i] = elems[i] * q;
		return res;
	}

	friend VecT operator* (T q, const VecT &b)
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.elems[i] = b.elems[i] * q;
		return res;
	}

	VecT &operator/= (T q)
	{
		for(int i=0; i<N; ++i)
			elems[i] /= q;
		return *this;
	}
	
	VecT operator/ (T q) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.elems[i] = elems[i] / q;
		return res;
	}

	T operator* (const VecT &b) const
	{
		T res(0);
		for(int i=0; i<N; ++i)
			res += elems[i] * b.elems[i];
		return res;
	}

	template<int K=N>
	typename std::enable_if<K == 3, VecT>::type
	operator% (const VecT &b) const
	{
		const T& x = VecSpecT<T,K>::x;
		const T& y = VecSpecT<T,K>::y;
		const T& z = VecSpecT<T,K>::z;

		return VecT(
			y*b.z - z*b.y,
			z*b.x - x*b.z,
			x*b.y - y*b.x
		);
	}

	VecT mul(const VecT &b) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.elems[i] = elems[i] * b.elems[i];
		return res;
	}

	double len() const
	{
		return sqrt(lenSqr());
	}

	T lenSqr() const
	{
		return *this * *this;
	}

	VecT norm() const
	{
		return (*this) * (1.0/len());
	}

	void normalize()
	{
		(*this) *= (1.0/len());
	}

	double dist(const VecT &b) const
	{
		return sqrt(distSqr(b));
	}

	T distSqr(const VecT &b) const
	{
		return lenSqr(*this - b);
	}

	bool operator< (const VecT &b) const
	{
		for(int i=0; i<N; ++i)
			if(elems[i] != b[i])
				return elems[i] < b[i];
		return false;
	}

	T &operator[] (int i)
	{
		assert(0 <= i && i < N);
		return elems[i];
	}

	const T &operator[] (int i) const
	{
		assert(0 <= i && i < N);
		return elems[i];
	}
};

template<class T, int N>
std::ostream& operator<<(std::ostream &s,const VecT<T,N> &r)
{
	s << "(";
	for(int i=0; i<N; ++i)
		s << (i == 0 ? "" : ",") << r[i];
	s << ")";
	return s;
}

typedef VecT<real,3> Vec3D;

#elif 1

template<class T, int N>
struct VecSpecT;


template<class T>
struct VecSpecT<T,2>
{
	T x, y;

	T* data() { return &x; }
	const T* data() const { return &x; }
};


template<class T>
struct VecSpecT<T,3>
{
	T x, y, z;

	T* data() { return &x; }
	const T* data() const { return &x; }
};


template<class T>
struct VecSpecT<T,4>
{
	T x, y, z, w;

	T* data() { return &x; }
	const T* data() const { return &x; }
};


template<class T, int N>
struct VecSpecT
{
	T elems[N];

	T* data() { return elems[0]; }
	const T* data() const { return elems[0]; }
};



template<class T, int N>
class VecT : public VecSpecT<T,N>
{
	static_assert(sizeof(VecSpecT<T,N>) == N*sizeof(T));

	using VecSpecT<T,N>::data;


	void init(int index, T a1)
	{
		data()[index] = a1;
	}

	template<typename... Args>
	void init(int index, T a1, Args... args)
	{
		data()[index] = a1;
		init(index+1, args...);
	}

public:
	VecT()
	{
		static_assert(sizeof(*this) == N*sizeof(T));
	}

	template<typename... Args>
	VecT(T e, Args... args)
	{
		static_assert(sizeof(*this) == N*sizeof(T));
		static_assert(sizeof...(Args) == N-1);
		init(0, e, args...);
		ASSERT(check());
	}


	VecT(T e)
	{
		for(int i=0; i<N; ++i)
			data()[i] = e;
		ASSERT(check());
	}

	template<int K, typename... Args>
	VecT(const VecT<T,K>& v, Args... args)
	{
		for(int i=0; i<K; ++i)
			data()[i] = v[i];
		init(K, args...);
		ASSERT(check());
	}

	void setZero()
	{
		for(int i=0; i<N; ++i)
			data()[i] = 0;
	}

	bool operator== (const VecT &b) const
	{
		for(int i=0; i<N; ++i)
			if(data()[i] != b.data()[i])
				return false;
		return true;
	}
	
	bool operator!= (const VecT &b) const
	{
		for(int i=0; i<N; ++i)
			if(data()[i] != b.data()[i])
				return true;
		return false;
	}
	
	VecT operator+= (const VecT &b)
	{
		for(int i=0; i<N; ++i)
			data()[i] += b.data()[i];
		return *this;
	}
	
	VecT operator+ (const VecT &b) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.data()[i] = data()[i] + b.data()[i];
		return res;
	}
	
	VecT operator-= (const VecT &b)
	{
		for(int i=0; i<N; ++i)
			data()[i] -= b.data()[i];
		return *this;
	}
	
	VecT operator- (const VecT &b) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.data()[i] = data()[i] - b.data()[i];
		return res;
	}

	VecT operator- () const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.data()[i] = -data()[i];
		return res;
	}
	
	VecT& operator*= (T q)
	{
		for(int i=0; i<N; ++i)
			data()[i] *= q;
		return *this;
	}
	
	VecT operator* (T q) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.data()[i] = data()[i] * q;
		return res;
	}

	friend VecT operator* (T q, const VecT &b)
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.data()[i] = b.data()[i] * q;
		return res;
	}

	VecT &operator/= (T q)
	{
		for(int i=0; i<N; ++i)
			data()[i] /= q;
		return *this;
	}
	
	VecT operator/ (T q) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.data()[i] = data()[i] / q;
		return res;
	}

	T operator* (const VecT &b) const
	{
		T res(0);
		for(int i=0; i<N; ++i)
			res += data()[i] * b.data()[i];
		return res;
	}

	template<int K=N>
	typename std::enable_if<K == 3, VecT>::type
	operator% (const VecT &b) const
	{
		const VecT& a = *this;

		return VecT(
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x
		);
	}

	VecT mul(const VecT &b) const
	{
		VecT res;
		for(int i=0; i<N; ++i)
			res.data()[i] = data()[i] * b.data()[i];
		return res;
	}

	real len() const
	{
		return sqrt(lenSqr());
	}

	T lenSqr() const
	{
		return *this * *this;
	}

	VecT norm() const
	{
		VecT res = (*this) * real(1.0/len());
		ASSERT(res.check());
		return res;
	}

	void normalize()
	{
		(*this) *= real(1.0/len());
		ASSERT(check());
	}

	real dist(const VecT &b) const
	{
		return sqrt(distSqr(b));
	}

	T distSqr(const VecT &b) const
	{
		return (*this - b).lenSqr();
	}

	bool operator< (const VecT &b) const
	{
		for(int i=0; i<N; ++i)
			if(data()[i] != b[i])
				return data()[i] < b[i];
		return false;
	}

	T &operator[] (int i)
	{
		ASSERT(0 <= i && i < N);
		return data()[i];
	}

	const T &operator[] (int i) const
	{
		ASSERT(0 <= i && i < N);
		return data()[i];
	}

	template<class T_=T>
	typename std::enable_if<std::is_floating_point<T_>::value,bool>::type
	check()
	{
		for(int i=0; i<N; ++i)
			if(!std::isfinite(data()[i]))
				return false;
		return true;
	}

	template<class T_=T>
	typename std::enable_if<!std::is_floating_point<T_>::value,bool>::type
	check()
	{
		return true;
	}
};

template<class T, int N>
std::ostream& operator<<(std::ostream &s,const VecT<T,N> &r)
{
	s << "(";
	for(int i=0; i<N; ++i)
		s << (i == 0 ? "" : ",") << r[i];
	s << ")";
	return s;
}

typedef VecT<real,2> Vec2D;
typedef VecT<real,3> Vec3D;
typedef VecT<real,4> Vec4D;

#endif