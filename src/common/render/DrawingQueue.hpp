#pragma once

#include <vector>
#include <stack>
#include "DrawCommand.hpp"


class DrawingQueue
{
public:
	struct TransformedCmd
	{
		TransformedCmd(const Matrix4& mat, const DrawCommand* cmd)
			: mat(mat)
			, cmd(cmd)
		{}

		Matrix4 mat;
		const DrawCommand* cmd;
	};

private:
	std::vector<TransformedCmd> cmds;
	std::stack<Matrix4> matStack;

public:
	DrawingQueue();
	~DrawingQueue();

	void pushMatrix(const Matrix4& mat);
	void popMatrix();

	void add(const DrawCommand* cmd);

	const std::vector<TransformedCmd>& getCmd() const { return cmds; }
};
