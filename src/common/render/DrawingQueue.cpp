#include "DrawingQueue.hpp"


DrawingQueue::DrawingQueue()
{
	matStack.push(Matrix4(1));
}


DrawingQueue::~DrawingQueue()
{
	ASSERT(matStack.size() == 1);
}


void DrawingQueue::pushMatrix(const Matrix4& mat)
{
	Matrix4 absMat = matStack.top() * mat;
	matStack.push(absMat);
}


void DrawingQueue::popMatrix()
{
	matStack.pop();

	ASSERT(!matStack.empty());
}


void DrawingQueue::add(const DrawCommand* cmd)
{
	cmds.emplace_back(matStack.top(), cmd);
}
