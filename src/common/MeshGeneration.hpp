#pragma once

#include "Vec.hpp"
#include "Mesh.hpp"


Mesh GenSphere(real r, int n, int k, const Vec3D& pos=Vec3D(0.,0.,0.));

void GenCone(Mesh& mesh, real r1, real r2, real h, int n, const Vec3D& pos=Vec3D(0.,0.,0.));
