#include "StringUtils.hpp"
#include "OpenGLDefs.hpp"
#include "OpenGLContext.hpp"
#include "ShaderVariable.hpp"


template<>
void ShaderVariableContainer::addVariable<int>(const std::string& name, const int& val)
{
	all.push_back({ShaderVariable::VariableType::GLSLUniform, ShaderVariable::ValueType::Int, name, val});
}

template<>
void ShaderVariableContainer::addVariable<std::vector<real>>(const std::string& name, std::vector<real> const &val)
{
	all.push_back({ShaderVariable::VariableType::GLSLAttribute, ShaderVariable::ValueType::VectorReal, name, val});
}

template<>
void ShaderVariableContainer::addVariable<std::vector<Vec2D>>(const std::string& name, std::vector<Vec2D> const &val)
{
	all.push_back({ShaderVariable::VariableType::GLSLAttribute, ShaderVariable::ValueType::VectorVec2, name, val});
}

template<>
void ShaderVariableContainer::addVariable<std::vector<Vec3D>>(const std::string& name, std::vector<Vec3D> const &val)
{
	all.push_back({ShaderVariable::VariableType::GLSLAttribute, ShaderVariable::ValueType::VectorVec3, name, val});
}

template<>
void ShaderVariableContainer::addVariable<std::vector<Vec4D>>(const std::string& name, std::vector<Vec4D> const &val)
{
	all.push_back({ShaderVariable::VariableType::GLSLAttribute, ShaderVariable::ValueType::VectorVec4, name, val});
}

template<>
void ShaderVariableContainer::addVariable<Material>(const std::string& name, const Material& val)
{
	all.push_back({ShaderVariable::VariableType::GLSLUniform, ShaderVariable::ValueType::Material, name, val});
}


int GLSLVariableTransmitter::getAttribLocation(const std::string& name, bool mustHave)
{
	int loc = glGetAttribLocation(shaderId, name.c_str());
	CHECK_GL_ERROR();
	if(loc < 0 && mustHave)
		throw std::runtime_error(sformat("Shader attrib \"%s\" not found", name.c_str()).c_str());
	return loc;
}


int GLSLVariableTransmitter::getUniformLocation(const std::string& name, bool mustHave)
{
	int loc = glGetUniformLocation(shaderId, name.c_str());
	CHECK_GL_ERROR();
	if(loc < 0 && mustHave)
		throw std::runtime_error(sformat("Shader uniform \"%s\" not found", name.c_str()).c_str());
	return loc;
}


template<>
void GLSLVariableTransmitter::setUniform<int>(int loc, const int& val)
{
	glUniform1i(loc, val);
	CHECK_GL_ERROR();
}

template<>
void GLSLVariableTransmitter::setUniform<real>(int loc, const real& val)
{
	glUniform1f(loc, (float)val);
	CHECK_GL_ERROR();
}

template<>
void GLSLVariableTransmitter::setUniform<VecT<float,3>>(int loc, const VecT<float,3>& val)
{
	glUniform3fv(loc, 1, &val[0]);
	CHECK_GL_ERROR();
}

template<>
void GLSLVariableTransmitter::setUniform<VecT<double,3>>(int loc, const VecT<double,3>& val)
{
	VecT<float,3> fval(static_cast<float>(val[0]), static_cast<float>(val[1]), static_cast<float>(val[2]));
	glUniform3fv(loc, 1, &fval[0]);
	CHECK_GL_ERROR();
}

template<>
void GLSLVariableTransmitter::setUniform<VecT<float,4>>(int loc, const VecT<float,4>& val)
{
	glUniform4fv(loc, 1, &val[0]);
	CHECK_GL_ERROR();
}

template<>
void GLSLVariableTransmitter::setUniform<VecT<double,4>>(int loc, const VecT<double,4>& val)
{
	VecT<float,4> fval(static_cast<float>(val[0]), static_cast<float>(val[1]), static_cast<float>(val[2]), static_cast<float>(val[3]));
	glUniform4fv(loc, 1, &fval[0]);
	CHECK_GL_ERROR();
}

template<>
void GLSLVariableTransmitter::setUniform<TransformationMatrixT<float, MatrixDataVecT>>(int loc, const TransformationMatrixT<float, MatrixDataVecT>& val)
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, val[0]);
	CHECK_GL_ERROR();
}

template<>
void GLSLVariableTransmitter::setUniform<TransformationMatrixT<double, MatrixDataVecT>>(int loc, const TransformationMatrixT<double, MatrixDataVecT>& val)
{
	float fv[16];
	for(int i=0; i<16; ++i)
		fv[i] = static_cast<float>(val[0][i]);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &fv[0]);
	CHECK_GL_ERROR();
}


void GLSLVariableTransmitter::setUniform(const std::string& name, const Material& val, bool mustHave)
{
	setUniform(name + ".ambient", val.ambient, mustHave);
	setUniform(name + ".diffuse", val.diffuse, mustHave);
	setUniform(name + ".specular", val.specular, mustHave);
	setUniform(name + ".shininess", val.specularExp, mustHave);
	setUniform(name + ".hasTexture", (int)val.hasTexture(), mustHave);

	if(val.hasTexture())
	{
		OpenGLContext::makeAndBindTexture(*val.texture);
		setUniform(name + "_sampler", 0, mustHave);
	}
}


template<>
void GLSLVariableTransmitter::setVertexAttribArray(int loc, const std::vector<int>& arr)
{
	setVertexAttribArray(loc, 1, GL_INT, &arr[0]);
}

template<>
void GLSLVariableTransmitter::setVertexAttribArray(int loc, const std::vector<real>& arr)
{
	setVertexAttribArray(loc, 1, GL_REAL, &arr[0]);
}

template<>
void GLSLVariableTransmitter::setVertexAttribArray(int loc, const std::vector<Vec2D>& arr)
{
	setVertexAttribArray(loc, 2, GL_REAL, &arr[0]);
}

template<>
void GLSLVariableTransmitter::setVertexAttribArray(int loc, const std::vector<Vec3D>& arr)
{
	setVertexAttribArray(loc, 3, GL_REAL, &arr[0]);
}

template<>
void GLSLVariableTransmitter::setVertexAttribArray(int loc, const std::vector<Vec4D>& arr)
{
	setVertexAttribArray(loc, 4, GL_REAL, &arr[0]);
}


void GLSLVariableTransmitter::setVertexAttribArray(int loc, int size, int type, const void* ptr)
{
	glVertexAttribPointer(loc, size, type, GL_FALSE, 0, ptr);
	glEnableVertexAttribArray(loc);
	CHECK_GL_ERROR();

	enabledVertexAttribs.push_back(loc);
}


void GLSLVariableTransmitter::setVariables(const ShaderVariableContainer& vars)
{
	for(const ShaderVariable& var : vars.all)
	{
		if(var.varType == ShaderVariable::VariableType::GLSLUniform)
		{
			if(var.valType == ShaderVariable::ValueType::Int)
				setUniform(var.name, std::get<int>(var.value));
			else if(var.valType == ShaderVariable::ValueType::Real)
				setUniform(var.name, std::get<real>(var.value));
			else if(var.valType == ShaderVariable::ValueType::Material)
				setUniform(var.name, std::get<Material>(var.value));
			else
				assert(0);
		}
		else if(var.varType == ShaderVariable::VariableType::GLSLAttribute)
		{
			if(var.valType == ShaderVariable::ValueType::VectorInt)
				setVertexAttribArray(var.name, std::get<std::vector<int>>(var.value));
			else if(var.valType == ShaderVariable::ValueType::VectorReal)
				setVertexAttribArray(var.name, std::get<std::vector<real>>(var.value));
			else if(var.valType == ShaderVariable::ValueType::VectorVec2)
				setVertexAttribArray(var.name, std::get<std::vector<Vec2D>>(var.value));
			else if(var.valType == ShaderVariable::ValueType::VectorVec3)
				setVertexAttribArray(var.name, std::get<std::vector<Vec3D>>(var.value));
			else if(var.valType == ShaderVariable::ValueType::VectorVec4)
				setVertexAttribArray(var.name, std::get<std::vector<Vec4D>>(var.value));
			else
				assert(0);
		}
		else
		{
			assert(0);
		}
	}
}


void GLSLVariableTransmitter::disableAttribs()
{
	for(int loc : enabledVertexAttribs)
		glDisableVertexAttribArray(loc);
	enabledVertexAttribs.clear();
}
