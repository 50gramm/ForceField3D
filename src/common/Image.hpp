#pragma once

class Image
{
public:
	enum PixelFormat
	{
		PF_GRAY,
		PF_RGB,
		PF_RGBA,
		PF_BGRA
	};

	typedef unsigned char Pixel;

	Image(const char* fileName)
	{
		load(fileName);
	}

	~Image();

	Pixel* getPixels() const { return pixels; }

	int getWidth() const { return width; }
	
	int getHeight() const { return height; }

	PixelFormat getPixelFormat() const { return format; }

protected:
	bool load(const char* fileName);

	int width = 0, height = 0;
	Pixel* pixels = 0;
	PixelFormat format;
};
