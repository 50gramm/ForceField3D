#pragma once

#include "Array.hpp"
#include "Vec.hpp"


class MotionEvent
{
public:
	enum Action
	{
		ACTION_DOWN,
		ACTION_UP,
		ACTION_DRAG
	};
	
	struct Point
	{
		Point()
		{}
		
		Point(float x, float y) :
			x(x), y(y)
		{}
		
		Point(int ix, int iy)
		{
			x = 1.0f*ix;
			y = 1.0f*iy;
		}

		float x, y;
	};
	
	MotionEvent(int width, int height)
		: maxX(width)
		, maxY(height)
	{}
		
	const Point &pointer(int i=0) const { return pointers[i]; }
	
	const Vec2D pnorm(int i=0) const; // Normalized coord, x: [-1,1]
	
	int pointerNum() const { return (int)pointers.size(); }
	
	Action action;
	int maxX, maxY;
	
	DynamicArray<Point> pointers;
};


class MotionObserver
{
public:
	MotionObserver();
	~MotionObserver();
	
	virtual bool onMotion(const MotionEvent& event) = 0;
};


class MotionSubject
{
	static DynamicArray<MotionObserver*> observers;

public:
	static void attach(MotionObserver* observer);
	static void detach(MotionObserver* observer);

	static bool notify(const MotionEvent& event);
};
