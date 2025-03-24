#pragma once
#include <memory>
#include "Image.hpp"


class Texture
{
public:
	std::shared_ptr<const Image> textureImage;

	unsigned int textureName = (unsigned int)-1;
	int contextId = -1;

	Texture(const std::shared_ptr<Image>& image)
		: textureImage(image)
	{}

	Texture(const char* imageFileName)
		: textureImage(std::make_shared<Image>(imageFileName))
	{}

	const Image* getImage() const { return textureImage.get(); }
};
