#include <cctype>
#include <cstring>
#include <filesystem>
#include "File.hpp"
#include "MeshObject.hpp"


static bool isNewLine(char c)
{
	return (0x0A <= c && c <= 0x0D);
}


MeshObject::MeshObject(const char* filePath)
{
	FileContent content = File::read(filePath, File::ASSET);
	const char* pc = content.c_str();
	const char* end = pc + content.get_size();
	ASSERT(end[0] == 0);

	while(pc < end)
	{
		while(pc < end && std::isspace(*pc))
			++pc;

		char token[9];
		token[0] = 0;

		sscanf(pc, "%8s", token);
		if(!strcmp(token, "v"))
		{
			parseVertex(pc);
		}
		else if(!strcmp(token, "vn"))
		{
			parseNormal(pc);
		}
		else if(!strcmp(token, "vt"))
		{
			parseTexCoord(pc);
		}
		else if(!strcmp(token, "f"))
		{
			parseFace(pc);
		}
		else if(!strcmp(token, "mtllib"))
		{
			char mtlib[999];
			sscanf(pc, "%8s %998s", token, mtlib);
			std::filesystem::path mtlibPath = std::filesystem::path(filePath).parent_path() / mtlib;
			materials.parseMaterials(mtlibPath.string().c_str());
		}
		else if(!strcmp(token, "usemtl"))
		{
			char name[999];
			sscanf(pc, "%8s %998s", token, name);
			currMaterial = materials.getMaterial(name);
		}

		while(pc++ < end && !isNewLine(*pc))
		{}
	}

	generateGlCommands();
}


void MeshObject::parseVertex(const char* line)
{
	char token[9];
	double x, y, z;

	int res = sscanf(line, "%8s %lf %lf %lf", token, &x, &y, &z);
	ASSERT(res == 4);
	verts.emplace_back(real(x), real(y), real(z));
}


void MeshObject::parseNormal(const char* line)
{
	char token[9];
	double x, y, z;

	int res = sscanf(line, "%8s %lf %lf %lf", token, &x, &y, &z);
	ASSERT(res == 4);
	norms.emplace_back(real(x), real(y), real(z));
}


void MeshObject::parseTexCoord(const char* line)
{
	char token[9];
	double x, y;

	int res = sscanf(line, "%8s %lf %lf", token, &x, &y);
	y = 1.0 - y;
	ASSERT(res == 3);
	texCoords.emplace_back(real(x), real(y));
}


void MeshObject::parseFace(const char* line)
{
	char token[9];

	int res = sscanf(line, "%8s", token);
	ASSERT(res == 1);

	const char* pc = line;
	faces.push_back(Face());
	Face& face = faces.back();
	face.material = currMaterial;

	pc = skipSpaces(pc);
	while(!std::isspace(*pc))
		++pc;

	while(*pc)
	{
		pc = skipSpaces(pc);
		if(isNewLine(*pc))
			break;

		int vi;
		int res = sscanf(pc, "%d", &vi);
		ASSERT(res == 1);
		face.vertexIndices.push_back(0 < vi ? vi-1 : verts.size() + vi);

		while(*pc != '/')
			++pc;
		pc = skipSpaces(++pc);
		if(*pc != '/')
		{
			int ti;
			res = sscanf(pc, "%d", &ti);
			ASSERT(res == 1);
			face.texCoordIndices.push_back(0 < ti ? ti-1 : texCoords.size() + ti);
			while(*pc != '/')
				++pc;
		}

		while(*pc != '/')
			++pc;
		pc = skipSpaces(++pc);
		if(*pc != '/')
		{
			int ni;
			res = sscanf(pc, "%d", &ni);
			ASSERT(res == 1);
			face.normalIndices.push_back(0 < ni ? ni-1 : norms.size() + ni);
			pc = skipSpaces(pc);
			while(std::isalnum(*++pc))
			{}
		}
	}
}


void MeshObject::generateGlCommands()
{
	DynamicArray<std::pair<const Material*,int>> faceOrder;
	for(int i=0; i<(int)faces.size(); ++i)
	{
		faceOrder.emplace_back(faces[i].material, i);
	}
	std::sort(faceOrder.begin(), faceOrder.end());

	for(auto& fo : faceOrder)
	{
		const Face& face = faces[fo.second];
		Mesh& glMesh = meshesByMat[face.material];

		for(int i=2; i<face.vertexIndices.size(); ++i)
		{
			glMesh.tris.push_back(glMesh.verts.size());
			glMesh.tris.push_back(glMesh.verts.size()+1);
			glMesh.tris.push_back(glMesh.verts.size()+2);

			glMesh.verts.push_back(verts[face.vertexIndices[0]]);
			glMesh.verts.push_back(verts[face.vertexIndices[i-1]]);
			glMesh.verts.push_back(verts[face.vertexIndices[i]]);

			if(!face.normalIndices.empty())
			{
				glMesh.norms.push_back(norms[face.normalIndices[0]]);
				glMesh.norms.push_back(norms[face.normalIndices[i-1]]);
				glMesh.norms.push_back(norms[face.normalIndices[i]]);
			}

			if(!face.texCoordIndices.empty())
			{
				glMesh.texCoords.push_back(texCoords[face.texCoordIndices[0]]);
				glMesh.texCoords.push_back(texCoords[face.texCoordIndices[i-1]]);
				glMesh.texCoords.push_back(texCoords[face.texCoordIndices[i]]);
			}
		}
	}

	for(const auto& it : meshesByMat)
		cmds.push_back(it.second.genDrawCmd(it.first));
}


void MeshObject::draw(DrawingQueue& drawingQueue)
{
	drawingQueue.pushMatrix(getPosition());

	for(const auto& cmd : cmds)
		drawingQueue.add(&cmd);

	drawingQueue.popMatrix();
}


void MeshObject::setShader(const std::string& name)
{
	for(auto& cmd : cmds)
		cmd.shaderName = name;
}


const char* MeshObject::skipSpaces(const char* pc)
{
	while(std::isblank(*pc))
		++pc;
	return pc;
}
