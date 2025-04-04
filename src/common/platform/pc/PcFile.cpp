#include <cstdio>
#include "Exception.hpp"
#include "Error.hpp"
#include "Platform.h"


size_t platform_read_file(const char *name, void **pbuff)
{
	const char *path = name;
	
	FILE *file = fopen(path, "rb");
	if(file == NULL)
	{
		throw Exception("Can not open file for read: %s\n", path);
	}
	
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	*pbuff = new char[size+1];
	((char*)(*pbuff))[size] = 0;
	fseek(file, 0, SEEK_SET);
	
	size_t res = fread(*pbuff, 1, size, file);
	if(res != size)
	{
		fclose(file);
		throw Exception("Can not read entire file: %s size: %ld read: %ld\n", path, size, (long)res);
	}
	
	if(fclose(file))
	{
		LOG_E("fclose error, file: %s\n", path);
	}

	return res;
}


void platform_write_file(const char *name, const void* buff, int size)
{
	FILE *file = fopen(name, "wb");
	if(file == NULL)
	{
		throw Exception("Can not open file for writing: %s\n", name);
	}
	
	int n = fwrite(buff, 1, size, file);
	if(n != size)
	{
		fclose(file);
		throw Exception("fwrite error, file: %s %d != %d\n", name, n, size);
	}
	
	if(fclose(file))
	{
		LOG_E("fclose error, file: %s\n", name);
	}
}
