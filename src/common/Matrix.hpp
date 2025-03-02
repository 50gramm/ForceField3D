#pragma once

#include <vector>
#include "Vec.hpp"
#include "Error.hpp"


template<class T, int N>
class MatrixDataVecT
{
	std::vector<T> elems;

public:
	MatrixDataVecT()
		: elems(N*N)
	{}

	const T* operator[](int i) const { ASSERT(0 <= i && i < N); return &elems[i*N]; }
	T*       operator[](int i)       { ASSERT(0 <= i && i < N); return &elems[i*N]; }
};


template<class T, int N>
class MatrixDataArrT
{
	T elems[N*N];

public:
	const T* operator[](int i) const { ASSERT(0 <= i && i < N); return &elems[i*N]; }
	T*       operator[](int i)       { ASSERT(0 <= i && i < N); return &elems[i*N]; }
};


template<class T, int N, class Data>
class MatrixT
{
protected:
	Data data;

	bool check() const
	{
		for(int i=0; i<N; ++i)
			for(int j=0; j<N; ++j)
				if(!std::isfinite(data[i][j]))
					return false;
		return true;
	}

public:
	MatrixT()
	{
#ifdef _DEBUG
		for(int i=0; i<N; ++i)
			for(int j=0; j<N; ++j)
				data[i][j] = std::numeric_limits<real>::infinity();
		ASSERT(!check());
#endif
	}

	MatrixT(T e)
	{
		for(int i=0; i<N; ++i)
			for(int j=0; j<N; ++j)
				data[i][j] = (i == j ? e : 0);
	}

	const T* operator[](int i) const { return data[i]; }
	T*       operator[](int i)       { return data[i]; }


	MatrixT& operator+=(const MatrixT& b)
	{
		for(int i=0; i<N*N; ++i)
			data[i] += b[i];
		return *this;
	}


	MatrixT operator+(const MatrixT& b) const
	{
		MatrixT res;
		for(int i=0; i<N*N; ++i)
			res.data[i] = data[i] + b[i];
		return res;
	}


	// MatrixT& operator*=(const MatrixT& b)
	// {
	// 	MatrixT res(0);

	// 	for(int i=0; i<N; ++i)
	// 		for(int j=0; j<N; ++j)
	// 			for(int k=0; k<N; ++k)
	// 				res[i][j] += data[i][k] * b[k][j];
	// 	*this = std::move(res);
	// 	return *this;
	// }


	// MatrixT operator*(const MatrixT& b) const
	// {
	// 	MatrixT res(0);

	// 	for(int i=0; i<N; ++i)
	// 		for(int j=0; j<N; ++j)
	// 			for(int k=0; k<N; ++k)
	// 				res[i][j] += data[i][k] * b[k][j];
	// 	return res;
	// }


	VecT<T,N> operator*(const VecT<T,N>& v)
	{
		VecT<T,N> res(0);

		for(int i=0; i<N; ++i)
			for(int j=0; j<N; ++j)
				res[i] += data[i][j] * v[j];

		return res;
	}


	MatrixT& transpose()
	{
		for(int i=0; i<N; ++i)
			for(int j=0; j<i; ++j)
				std::swap(data[i][j], data[j][i]);
		return *this;
	}
};


template<class Matrix, class T, int N, class Data>
Matrix& operator*=(Matrix& a, const MatrixT<T,N,Data>& b)
{
	Matrix res(0);

	for(int i=0; i<N; ++i)
		for(int j=0; j<N; ++j)
			for(int k=0; k<N; ++k)
				res[i][j] += a[i][k] * b[k][j];
	a = std::move(res);
	return a;
}


template<class Matrix, class T, int N, class Data>
Matrix operator*(const Matrix& a, const MatrixT<T,N,Data>& b)
{
	Matrix res(0);

	for(int i=0; i<N; ++i)
		for(int j=0; j<N; ++j)
			for(int k=0; k<N; ++k)
				res[i][j] += a[i][k] * b[k][j];
	return res;
}

