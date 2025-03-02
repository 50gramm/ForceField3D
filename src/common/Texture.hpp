#pragma once
#include "Image.hpp"


class Texture
{
public:
	Image* textureImage;

	unsigned int textureName = (unsigned int)-1;
	int contextId = -1;

	Texture(const char* imageFileName)
	{
		textureImage = new Image(imageFileName);
	}
};
