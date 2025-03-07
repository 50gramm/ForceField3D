
#include "MathUtils.hpp"
#include "ShaderManager.hpp"
#include "DefaultShader.hpp"
#include "ForceFieldState.hpp"
#include "ForceLines.hpp"
#include "ForceFieldOp.hpp"


static DrawingProgramRegistrator<DefaultShader> shaderReg("forcelines");


static DynamicArray<Vec3D> genPointsOnSphere(int n, unsigned seed)
{
	DynamicArray<Vec3D> points(n);
	srand(seed);

	for(Vec3D& p : points)
	{
		do {
			p = Vec3D(2*frand()-1, 2*frand()-1, 2*frand()-1);
		} while(isNearZero(p));
		p.normalize();
	}

	if(n <= 1)
		return points;

	for(int iter=0; iter<99; ++iter)
	{
		DynamicArray<Vec3D> forces(n, Vec3D(0,0,0));

		for(int i=0; i<n; ++i)
		{
			for(int j=0; j<i; ++j)
			{
				Vec3D dr = (points[i] - points[j]);
				real len = dr.len();
				Vec3D f = dr / (len*len*len);
				forces[i] += f;
				forces[j] -= f;
			}
		}

		for(int i=0; i<n; ++i)
		{
			points[i] += forces[i] * 0.1 / (n-1);
			points[i].normalize();
		}
	}

	return points;
}


ForceLines::ForceLines(const ForceFieldState& state, const VisualSettings& visSettings)
	: state(state)
	, visSettings(visSettings)
{
	drawCmd.shaderName = "forcelines";
	drawCmd.vars.addVariable("uMaterial", *visSettings.getMaterial("ForceLine"));
	drawCmd.vars.addVariable("uMaterial2", *visSettings.getMaterial("ForceLineNegative"));
}


void ForceLines::generateLineMesh(const ForceLine& line)
{
	const int circRes = visSettings["ForceLines"]["CircleSegments"].get<int>();
	const real radius = visSettings["ForceLines"]["CircleRadius"].get<real>();

	DynamicArray<Vec3D> circVert(circRes);
	for(int i=0; i<circRes; ++i)
		circVert[i] = Vec3D(cos(i*M_PI*2/circRes), sin(i*M_PI*2/circRes), 0.0);

	int index0 = linesMesh.verts.size();

	real totalLength = 0.0;
	{
		int segNum = 0;
		Vec3D prevPos;
		for(const Gradient& grad : line.points)
		{
			if(0 < segNum)
				totalLength += (grad.pos - prevPos).len();
			segNum += 1;
			prevPos = grad.pos;
		}
	}
	real tangentFactor = (line.finishChargeValue == 0 ? 0.5 : 1.0) / totalLength;

	int segNum = 0;
	real length = 0.0;
	Vec3D prevPos;
	for(const Gradient& grad : line.points)
	{
		if(0 < segNum)
			length += (grad.pos - prevPos).len();

		Matrix4 mat({Vec3D(0,0,1), grad.dir}, grad.pos, radius);
		for(const Vec3D& p : circVert)
		{
			linesMesh.verts.push_back(mat * p);
			linesMesh.norms.push_back((mat * p - mat.getTranslation()).norm());
			tangential.push_back(length);
			tangentialNormalized.push_back(line.startChargeValue < 0 ? length*tangentFactor : 1.0 - length*tangentFactor);
		}
		segNum += 1;
		prevPos = grad.pos;
	}

	for(int iSeg=0; iSeg+1<segNum; ++iSeg) //if(iSeg%2 == 0)
	{
		for(int j=0; j<circRes; ++j)
		{
			linesMesh.tris.push_back(index0 + iSeg*circRes + j);
			linesMesh.tris.push_back(index0 + iSeg*circRes + (j+1)%circRes);
			linesMesh.tris.push_back(index0 + (iSeg+1)*circRes + j);

			linesMesh.tris.push_back(index0 + iSeg*circRes + (j+1)%circRes);
			linesMesh.tris.push_back(index0 + (iSeg+1)*circRes + (j+1)%circRes);
			linesMesh.tris.push_back(index0 + (iSeg+1)*circRes + j);
		}
	}
}


ForceLines::ForceLine ForceLines::generateLine(const PointCharge& source, const Vec3D dir, bool positiveDir) const
{
	const real lineStep = visSettings["ForceLines"]["LineSegmentSize"].get<real>();

	ForceLine line;
	line.startChargeValue = source.charge;
	line.points.emplace_back(source.r, dir);
	Vec3D pos = source.r + dir * sqrt(fabs(source.charge)) * 0.01;

	while(line.points.size() < 399)
	{
		bool finished = false;
		Vec3D E(0,0,0);
		Vec3D dE(0,0,0);
		for(const PointCharge& charge : state.charges)
		{
			Vec3D dr = (pos - charge.r);
			real dist = dr.len();
			if(charge.isNegative() == positiveDir && dist < PointCharges::getRadius(charge.charge))
			{
				line.finishChargeValue = charge.charge;
				finished |= true;
			}
			E  += dr * ((positiveDir ? 1 : -1) * charge.charge / (dist*dist*dist));
			dE += dr * ((positiveDir ? 1 : -1) * charge.charge / (SQR(SQR(dist))));
		}
		Vec3D dir = E.norm();
		dE -= dir * (dir * dE);
		line.points.emplace_back(pos, dir);
		if(finished || E.lenSqr() < SQR(1E-4))
			break;
		pos += line.points.back().dir * lineStep;
	}
	
	return line;
}


void ForceLines::regenerate()
{
	lines.clear();
	tangential.clear();
	tangentialNormalized.clear();
	linesMesh = Mesh();

	real sumCharge = 0;
	for(const PointCharge& charge : state.charges)
		sumCharge += charge.charge;
	bool positive = (0 < sumCharge);

	for(const PointCharge& charge : state.charges) if(charge.isPositive() == positive)
	{
		int n = 8 * fabs(charge.charge);
		DynamicArray<Vec3D> starts = genPointsOnSphere(n, charge.getId());
		for(const Vec3D& p : starts)
		{
			lines.push_back(generateLine(charge, p, positive));
			generateLineMesh(lines.back());
		}
	}

	linesMesh.fillDrawCmd(drawCmd);
	drawCmd.vars.addVariable<std::vector<real>>("aTangentialNormalized", tangentialNormalized);

	generatedStateId = state.stateId;
}


void ForceLines::draw(DrawingQueue& drawing)
{
	if(generatedStateId != state.stateId)
		regenerate();

	drawing.add(&drawCmd);
}
