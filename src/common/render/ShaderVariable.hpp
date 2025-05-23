#pragma once

#include <variant>
#include <string>
#include <map>
#include "Array.hpp"
#include "TransformationMatrix.hpp"
#include "Material.hpp"


struct ShaderVariable
{
	enum class VariableType
	{
		GLSLAttribute,
		GLSLUniform,
	};

	enum class ValueType
	{
		Int,
		Real,
		Vec2D,
		Vec3D,
		Material,
		VectorInt,
		VectorReal,
		VectorVec2,
		VectorVec3,
		VectorVec4,
		VectorShaderVariable,
	};

	VariableType varType;
	ValueType valType;

	std::string name;

	std::variant<
		int, real, Vec2D, Vec3D, Matrix4,
		Material,
		std::vector<int>, std::vector<real>, std::vector<Vec2D>, std::vector<Vec3D>, std::vector<Vec4D>,
		std::vector<ShaderVariable>
	> value;

	mutable unsigned valBufferId = 0;

	template<class T>
	ShaderVariable(const std::string& name, const T& val);

	ShaderVariable(ShaderVariable& other) = delete;
	ShaderVariable& operator=(ShaderVariable& other) = delete;
	ShaderVariable(ShaderVariable&& other) = default;
	ShaderVariable& operator=(ShaderVariable&& other) = delete;

	~ShaderVariable();
};


class ShaderVariableContainer
{
public:
	std::vector<ShaderVariable> all;

	template<class T>
	void addVariable(const std::string& name, const T& val)
	{
		all.emplace_back(name, val);
	}
};



class GLSLVariableTransmitter
{
	int shaderId = -1;
	std::map<std::string, int> attributeLocs;
	std::vector<int> enabledVertexAttribs;

	int getAttribLocation(const std::string& name);

	int getUniformLocation(const std::string& name);

	template<class T>
	void setVertexAttribArray(int loc, const std::vector<T>& arr);

	void setVertexAttribArray(int loc, int size, int type, const void* ptr);

	void bindArrayBuffer(size_t size, const void* ptr, unsigned& bufferId);
	
public:
	GLSLVariableTransmitter()
	{}


	GLSLVariableTransmitter(int shaderId)
		: shaderId(shaderId)
	{}


	template<class T>
	void setVertexAttribArray(const std::string& name, unsigned& bufferId, const std::vector<T>& arr)
	{
		int loc = getAttribLocation(name);
		if(loc < 0 || arr.empty())
			return;

		bindArrayBuffer(arr.size()*sizeof(T), &arr[0], bufferId);
		
		setVertexAttribArray(loc, arr);
	}


	void setUniform(const std::string& name, const Material& val);

	template<class T>
	void setUniform(const std::string& name, const T& val)
	{
		setUniform(getUniformLocation(name), val);
	}


	template<class T>
	void setUniform(int loc, const T& val)
	{
		if(0 <= loc)
			setUniformInner(loc, val);
	}

	template<class T>
	void setUniformInner(int loc, const T& val);

	void setVariables(const ShaderVariableContainer& vars);

	void disableAttribs();
};
