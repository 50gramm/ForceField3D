#pragma once

#include <string>
#include <memory>


class FileContent
{
	size_t size;
	std::unique_ptr<char[]> data;

public:
	FileContent(size_t size, void* data)
		: size(size)
		, data(static_cast<char*>(data))
	{}

	const void* get() const { return data.get(); }

	const char* c_str() const { return data.get(); }

	size_t get_size() const { return size; }
};


class File
{
public:
	enum SpecialFolder
	{
		ASSET, // Read only
		USER,  // Read/Write
		SPEC_FOLDER_NUM
	};

	static void set_folder(SpecialFolder folder, const std::string& name);

	static FileContent read(const char *file_name, SpecialFolder folder);
};
