#pragma once

#include <string>
#include "Vec.hpp"
#include "Texture.hpp"


class Material
{
public:
	typedef VecT<real,4> Color;

	Material() {} // TODO: Delete this

	Material(const std::string& name) :
		name(name)
	{}

	bool hasTexture() const { return texture != nullptr; }

	void setTexture(const std::shared_ptr<Texture>& tex) { texture = tex; }

	bool isTransparent() const
	{
		return
			(ambient.w < 1. || diffuse.w < 1. || specular.w < 1.) ||
			(texture && texture->getImage()->getPixelFormat() == Image::PF_RGBA)
		;
	}

	std::string name;
	Color ambient = {0.7, 0.7, 0.7, 1.0};
	Color diffuse = {0.7, 0.7, 0.7, 1.0};
	Color specular = {0.5, 0.5, 0.5, 1.0};
	real specularExp = 10.0;
	std::shared_ptr<Texture> texture;
};
