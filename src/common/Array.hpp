#pragma once

#include <vector>
#include <algorithm>
#include <numeric>


template <class T> class Vector : public std::vector<T>
{
  public:
	// Constructions
	Vector()
	{
	}


	template <class... P>
	Vector(P... args)
		: std::vector<T>(args...)
	{
	}


	Vector(std::initializer_list<T> l)
		: std::vector<T>(l)
	{
	}


	// Create multi-dimensional vector, eg. two dim 10x20 vector: Vector<float>::create(10, 20, 0.0);
	template <class... Args> static auto create(size_t n, Args... args)
	{
		auto sub_vec	= create(args...);
		using SubVector = decltype(sub_vec);
		return Vector<SubVector>(n, sub_vec);
	}


	template <class Q> static Vector<Q> create(size_t n, const Q &val)
	{
		return Vector<Q>(n, val);
	}


	// <algorithm> equivalent functions
	template <class... Args> void sort(Args... args)
	{
		std::sort(this->begin(), this->end(), args...);
	}


	template <class... Args> auto find_if(Args... args) const
	{
		return std::find_if(this->begin(), this->end(), args...);
	}


	template <class... Args> auto min_element(Args... args) const
	{
		return std::min_element(this->begin(), this->end(), args...);
	}


	template <class... Args> auto max_element(Args... args) const
	{
		return std::max_element(this->begin(), this->end(), args...);
	}


	template <class... Args> T accumulate(Args... args) const
	{
		return std::accumulate(this->begin(), this->end(), args...);
	}


	T reverse()
	{
		return std::reverse(this->begin(), this->end());
	}

	// Manipulation
	void add(const T& x)
	{
		this->push_back(x);
	}

	void erase_at(int i)
	{
		std::vector<T>::erase(this->begin() + i);
	}

	template<class Arr>
	void append(const Arr &other)
	{
		this->insert(this->end(), other.begin(), other.end());
	}

	void remove_one(const T &value)
	{
		const typename std::vector<T>::const_iterator it = std::find(this->begin(), this->end(), value);
		if(it != this->end())
		{
			this->erase(it);
		}
	}

	void remove_one_unstable(const T &value)
	{
		const typename std::vector<T>::iterator it = std::find(this->begin(), this->end(), value);
		if(it != this->end())
		{
			std::swap(*it, this->back());
			this->pop_back();
		}
	}

	void remove_all(const T &value)
	{
		std::vector<T>::erase(std::remove(this->begin(), this->end(), value), this->end());
	}
};

template <class T> void swap(Vector<T> &a, Vector<T> &b)
{
	a.swap(b);
}

#define DynamicArray Vector
