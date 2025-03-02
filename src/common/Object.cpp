#include "DrawingQueue.hpp"
#include "Object.hpp"


void Object3D::addChild(IVisual* vis)
{
	children.push_back(vis);
	Object3D* obj = dynamic_cast<Object3D*>(vis);
	if(obj)
		obj->parent = this;
}


Matrix4 Object3D::getAbsMat() const
{
	if(parent)
		return parent->getAbsMat() * getPosition();
	return getPosition();
}


void Object3D::draw(DrawingQueue& drawing)
{
	drawing.pushMatrix(getPosition());

	for(IVisual* vis : children)
		vis->draw(drawing);
	
	drawing.popMatrix();
}
