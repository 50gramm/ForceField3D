#include <cstdlib>
#include "Exception.hpp"
#include "lodepng/lodepng.h"
#include "File.hpp"
#include "Error.hpp"
#include "StringUtils.hpp"
#include "Image.hpp"


Image::~Image()
{
	free(pixels);
}


bool Image::load(const char* file)
{
	if(!(3 < strlen(file) && !strcmp(file + strlen(file) - 4, ".png")))
	{
		throw Exception("Invalid image name, only png supported: %s", file);
	}

	FileContent content = File::read(file);

	unsigned w, h;
	LodePNGState state;
	lodepng_state_init(&state);
	lodepng_inspect(&w, &h, &state, (unsigned char*)content.get(), content.get_size());
	state.info_raw.colortype = (lodepng_is_alpha_type(&state.info_png.color) ? LCT_RGBA : LCT_RGB);
	unsigned err = lodepng_decode(&pixels, &w, &h, &state, (unsigned char*)content.get(), content.get_size());
	if(err)
	{
		throw Exception("Failed to load image: %s err: %u", file, err);
	}
	width = w;
	height = h;

	if(state.info_raw.colortype == LCT_RGBA && state.info_raw.bitdepth == 8)
		format = PixelFormat::PF_RGBA;
	else if(state.info_raw.colortype == LCT_RGB && state.info_raw.bitdepth == 8)
		format = PixelFormat::PF_RGB;
	else
		throw Exception("Unsupported image format in: %s", file);
	
	return true;
}
