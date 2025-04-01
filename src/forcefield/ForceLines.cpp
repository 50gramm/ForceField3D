#include "Tracer.hpp"
#include "MathUtils.hpp"
#include "ShaderManager.hpp"
#include "DefaultShader.hpp"
#include "ForceFieldState.hpp"
#include "ForceFieldOp.hpp"
#include "ForceLines.hpp"


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
}


void ForceLines::generateLineMesh(const ForceLine& line)
{
	TRACE_FUNCTION

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
	Gradient prevGrad = {{}, Vec3D(0,0,1)};
	Matrix4 fullRot(1);
	for(const Gradient& grad : line.points)
	{
		if(0 < segNum)
			length += (grad.pos - prevGrad.pos).len();

		Matrix4 rot({prevGrad.dir, grad.dir}, Vec3D(0,0,0));
		fullRot = rot * fullRot;
		Matrix4 mat = fullRot;
		mat.scale(radius).setTranslation(grad.pos);
		for(const Vec3D& p : circVert)
		{
			linesMesh.verts.push_back(mat * p);
			linesMesh.norms.push_back((mat * p - mat.getTranslation()).norm());
			tangential.push_back(length);
			tangentialNormalized.push_back(line.startChargeValue < 0 ? length*tangentFactor : 1.0 - length*tangentFactor);
		}
		segNum += 1;
		prevGrad = grad;
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
	TRACE_FUNCTION
	
	const real lineAccuracy = visSettings["ForceLines"]["LineAccuracy"].get<real>();

	ForceLine line;
	line.startChargeValue = source.charge;
	line.points.push_back({source.r, dir});
	Vec3D pos = source.r + dir * PointCharges::getRadius(source.charge) / 2;
	real step = 1;

	while(line.points.size() < 399)
	{
		bool finished = false;
		Vec3D E(0,0,0);
		Vec3D dE(0,0,0);
		for(const PointCharge& charge : state.getCharges())
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
		Vec3D eDir = E.norm();
		dE -= dir * (dir * dE);
		line.points.push_back({pos, eDir});
		if(finished || E.lenSqr() < SQR(1E-4))
			break;

		Vec3D nextPos;
		do {
			nextPos = pos + eDir * step;
			step /= 2.0;
		} while(lineAccuracy < (state.getE(nextPos).norm() % eDir).len() * step);
		step *= 4.0;

		pos = nextPos;
	}
	
	return line;
}


void ForceLines::regenerate()
{
	int linesPerCharge = visSettings["ForceLines"]["LinesPerCharge"];

	lines.clear();
	tangential.clear();
	tangentialNormalized.clear();
	linesMesh = Mesh();

	real sumCharge = 0;
	for(const PointCharge& charge : state.getCharges())
		sumCharge += charge.charge;
	bool positive = (0 < sumCharge);

	for(const PointCharge& charge : state.getCharges()) if(charge.isPositive() == positive)
	{
		int n = linesPerCharge * fabs(charge.charge);
		DynamicArray<Vec3D> starts = genPointsOnSphere(n, 888);
		for(const Vec3D& p : starts)
		{
			lines.push_back(generateLine(charge, p, positive));
			generateLineMesh(lines.back());
		}
	}

	drawCmd = linesMesh.genDrawCmd();
	drawCmd.shaderName = "forcelines";
	drawCmd.vars.addVariable("uMaterial", *visSettings.getMaterial("ForceLine"));
	drawCmd.vars.addVariable("uMaterial2", *visSettings.getMaterial("ForceLineNegative"));

	drawCmd.vars.addVariable<std::vector<real>>("aTangentialNormalized", tangentialNormalized);

	generatedStateId = state.getId();
}


void ForceLines::draw(DrawingQueue& drawing)
{
	if(!show)
		return;

	if(generatedStateId != state.getId())
		regenerate();

	drawing.add(&drawCmd);
}


bool ForceLines::onKey(const KeyEvent& event)
{
	if(event.action == KeyEvent::ACTION_DOWN && event.keyCode == 'f')
	{
		show = !show;
		return true;
	}

	return false;
}