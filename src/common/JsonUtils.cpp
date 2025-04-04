#include "File.hpp"
#include "JsonUtils.hpp"


Json parseJsonFile(const char* filePath)
{
	return nlohmann::json::parse(File::read(filePath).c_str(), nullptr, true, true);
}


void dumpToJsonFile(const Json& jObject, const char* filePath)
{
	std::string sObject = jObject.dump(1, '\t');
	File::write(filePath, sObject.c_str(), sObject.size());
}


Json dumpMatrixToJson(const Matrix4& mat)
{
	std::vector<real> matElements(mat[0], mat[0]+16);
	return matElements;

}


Matrix4 parseJsonMatrix(const Json& jObject)
{
	Matrix4 mat;
	std::vector<real> matElements = jObject;
	for(int i=0; i<16; ++i)
		mat[0][i] = matElements[i];
	return mat;
}
