#pragma once

#include "Matrix.hpp"


template<class T>
class Rotation3DData
{
public:
	using Vec = VecT<T,3>;
	Vec dir;
	T sinus = 0, cosinus = 1;

	Rotation3DData() {}

	Rotation3DData(const Vec& dir, T angle)
		: dir(dir.norm())
		, sinus(sin(angle))
		, cosinus(cos(angle))
	{
	}

	Rotation3DData(const Vec& from, const Vec& to)
	{
		Vec v1 = from.norm();
		Vec v2 = to.norm();
		Vec v21 = v2 % v1;
		sinus = v21.len();
		cosinus = v1*v2;
		dir = v21.norm();
	}
};


template<class T, template<class,int> class MatrixDataT>
class TransformationMatrixT : public MatrixT<T, 4, MatrixDataT<T,4>>
{
	constexpr static int N = 4;
	constexpr static int K = N-1;
	using MatrixT<T, N, MatrixDataT<T,N>>::data;
	using Vec = VecT<T,K>;

public:
	TransformationMatrixT() {}

	
	TransformationMatrixT(T e)
		: MatrixT<T, N, MatrixDataT<T,N>>(e)
	{
	}


	TransformationMatrixT(const Rotation3DData<T>& rot, const Vec& trans, real scale = 1.0)
	{
		if(rot.sinus == 0)
		{
			*this = TransformationMatrixT(scale);
			data[K][K] = 1;
		}
		else
		{
			for(int i=0; i<K; ++i)
			{
				Vec v(0.f, 0.f, 0.f);
				v[i] = 1.f;
				Vec v1 = rot.dir % v;
				Vec v2 = rot.dir % v1;
				if(v1.lenSqr() > EPS*EPS)
					v1.normalize();
				v += v1 * v2.len() * rot.sinus + v2 * (1.f - rot.cosinus);
				v *= scale;
				for(int j=0; j<K; ++j)
					data[i][j] = v[j];
			}

			for(int j=0; j<K; ++j)
				data[K][j] = 0;
			data[K][K] = 1;
		}

		setTranslation(trans);
		ASSERT(this->check());
	}


	Vec operator*(const Vec& v) const
	{
		Vec res(0);

		for(int i=0; i<K; ++i)
			for(int j=0; j<N; ++j)
				res[i] += (j < K ? data[i][j] * v[j] : data[i][j]);

		return res;
	}


	TransformationMatrixT& setTranslation(const Vec& vec)
	{
		for(int i=0; i<K; ++i)
			data[i][K] = vec[i];
		return *this;
	}


	Vec getTranslation() const
	{
		Vec res;
		for(int i=0; i<K; ++i)
			res[i] = data[i][K];
		return res;
	}


	TransformationMatrixT& scale(T scale)
	{
		for(int i=0; i<K; ++i)
			for(int j=0; j<K; ++j)
				data[i][j] *= scale;
		return *this;
	}

	TransformationMatrixT inverse() const
	{
		TransformationMatrixT res(1);

		for(int i=0; i<K; ++i)
		{
			T sqrSum = 0;
			for(int j=0; j<K; ++j)
				sqrSum += data[i][j]*data[i][j];
			for(int j=0; j<K; ++j)
				res[j][i] = data[i][j] / sqrSum;
		}

		TransformationMatrixT trans({}, -getTranslation());

		res *= trans;

		ASSERT(res.check());

		return res;
	}


	Vec rotateVec(const Vec& vec) const
	{
		Vec res(0);

		for(int i=0; i<K; ++i)
			for(int j=0; j<K; ++j)
				res[i] += data[i][j] * vec[j];

		return res;
	}
};

using Matrix4 = TransformationMatrixT<real, MatrixDataVecT>;
