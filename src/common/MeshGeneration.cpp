#include <cmath>
#include "MeshGeneration.hpp"


void GenSphere(DynamicArray<Vec3D>& verts, DynamicArray<Vec3D>& norms, DynamicArray<int>& indices, real r, int n, int k, const Vec3D& pos)
{
	real alfa = M_PI/n, beta= 2*M_PI/k;
	int io = (int)verts.size();

	verts.add(pos + Vec3D(0,r,0));
	norms.add({0,1,0});

	for(int i=1; i<n; ++i)
	{
		for(int j=0; j<k; ++j)
		{
			Vec3D vec(
				sin(i*alfa)*cos(j*beta + i*beta/2),
				cos(i*alfa),
				sin(i*alfa)*sin(j*beta + i*beta/2));
			verts.add(pos + r*vec);
			norms.add(vec);
		}
	}

	verts.add(pos + Vec3D(0,-r,0));
	norms.add({0,-1,0});

	for(int j=0; j<k; ++j)
	{
		indices.add(io);
		indices.add(io + 1 + (j+1)%k);
		indices.add(io + 1 + j);
	}

	for(int i=1; i<n-1; ++i)
	{
		for(int j=0; j<k; ++j)
		{
			indices.add(io + 1 + (i-1)*k + j);
			indices.add(io + 1 + (i-1)*k + (j+1)%k);
			indices.add(io + 1 + i*k + j);

			indices.add(io + 1 + (i-1)*k + j);
			indices.add(io + 1 + i*k + j);
			indices.add(io + 1 + i*k + (j-1+k)%k);
		}
	}

	for(int j=0; j<k; ++j)
	{
		indices.add(io + 1 + (n-1)*k);
		indices.add(io + 1 + (n-2)*k + j);
		indices.add(io + 1 + (n-2)*k + (j+1)%k);
	}
}


Mesh GenSphere(real r, int n, int k, const Vec3D& pos)
{
	Mesh mesh;
	GenSphere(mesh.verts, mesh.norms, mesh.tris, r, n, k, pos);
	return mesh;
}


void GenCone(Mesh& mesh, real r1, real r2, real h, int n, const Vec3D& pos)
{
	real alpha = M_PI/n;
	int io = (int)mesh.verts.size();

	for(int i=0; i<n; ++i)
	{
		real sa = sin(i*alpha);
		real ca = cos(i*alpha);
		mesh.verts.add(pos + Vec3D(r1*ca,  0, r1*sa));
		mesh.verts.add(pos + Vec3D(r2*ca, -h, r2*sa));

		Vec3D norm(h*ca, (r2-r1), h*sa);
		norm.normalize();
		mesh.norms.add(norm);
		mesh.norms.add(norm);
	}

	for(int i=0; i<n; ++i)
	{
		mesh.tris.add(io + 2*i);
		mesh.tris.add(io + 2*((i+1)%n));
		mesh.tris.add(io + 2*i+1);

		mesh.tris.add(io + 2*i+1);
		mesh.tris.add(io + 2*((i+1)%n));
		mesh.tris.add(io + 2*((i+1)%n)+1);
	}
}
