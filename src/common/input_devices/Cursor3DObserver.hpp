#pragma once

#include "Array.hpp"
#include "TransformationMatrix.hpp"


struct Cursor3DEvent
{
	enum EventType
	{
		EVENT_MOVE,
		EVENT_DOWN,
		EVENT_UP
	};

	EventType type;
	int button;
	Matrix4 mat;
	Vec3D pos;
};


class Cursor3DObserver
{
public:
	Cursor3DObserver();
	~Cursor3DObserver();
	
	virtual bool onCursor3D(const Cursor3DEvent& event) = 0;
};


class Cursor3DSubject
{
	static DynamicArray<Cursor3DObserver*> observers;

public:
	static void attach(Cursor3DObserver* observer);
	static void detach(Cursor3DObserver* observer);

	static bool notify(const Cursor3DEvent& event);
};
