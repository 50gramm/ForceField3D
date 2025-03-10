#pragma once
#include <nlohmann/json.hpp>
#include "Json.hpp"
#include "TransformationMatrix.hpp"


Json parseJsonFile(const char* filePath);
void dumpToJsonFile(const Json& jObject, const char* filePath);

Json dumpMatrixToJson(const Matrix4& mat);
Matrix4 parseJsonMatrix(const Json& jObject);
