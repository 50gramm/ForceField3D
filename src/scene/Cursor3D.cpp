#include "DrawingQueue.hpp"
#include "MeshGeneration.hpp"
#include "Cursor3D.hpp"


Cursor3D::Cursor3D()
	: materials("{assets}/cursor.mtl")
{
	GenCone(mesh, 0.00, 0.01, 0.02, 12, Vec3D(0,-0.0,0));
	GenCone(mesh, 0.01, 0.02, 0.02, 12, Vec3D(0,-0.02,0));
	GenCone(mesh, 0.02, 0.03, 0.02, 12, Vec3D(0,-0.04,0));
	GenCone(mesh, 0.03, 0.04, 0.02, 12, Vec3D(0,-0.06,0));
	GenCone(mesh, 0.015, 0.015, 0.10, 12, Vec3D(0,-0.066,0));
	cmd = mesh.genDrawCmd(materials.getMaterial("DefaultCursor"));
}


bool Cursor3D::onCursor3D(const Cursor3DEvent& event)
{
	setPosition(event.mat);
	return false;
}


void Cursor3D::draw(DrawingQueue& drawing)
{
	drawing.pushMatrix(getPosition());

	drawing.add(&cmd);
	
	drawing.popMatrix();
}
