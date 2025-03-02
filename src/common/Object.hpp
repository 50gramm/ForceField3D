#pragma once

#include "Array.hpp"
#include "TransformationMatrix.hpp"

class DrawingQueue;


class IVisual
{
public:
	virtual ~IVisual() {}
	virtual void draw(DrawingQueue& drawing) = 0;
};


class Object3D : public IVisual
{
	Matrix4 position = Matrix4(1);
	DynamicArray<IVisual*> children;
	Object3D* parent = nullptr;

public:
	const Matrix4& getPosition() const { return position; }

	void setPosition(const Matrix4& pos) { position = pos; }

	void addChild(IVisual* vis);

	Matrix4 getAbsMat() const;

	virtual void draw(DrawingQueue& drawing) override;
};
