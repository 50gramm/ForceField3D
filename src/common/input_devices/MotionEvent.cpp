#include "MotionEvent.hpp"

const Vec2D MotionEvent::pnorm(int i) const
{
	return Vec2D((pointers[i].x - maxX/2.0) * 2.0 / maxX, (pointers[i].y - maxY/2.0)* 2.0 / maxX);
}

MotionObserver::MotionObserver()
{
	MotionSubject::attach(this);
}


MotionObserver::~MotionObserver()
{
	MotionSubject::detach(this);
}



DynamicArray<MotionObserver*> MotionSubject::observers;


void MotionSubject::attach(MotionObserver* observer)
{
	observers.add(observer);
}


void MotionSubject::detach(MotionObserver* observer)
{
	observers.remove_all(observer);
}


bool MotionSubject::notify(const MotionEvent& event)
{
	for(MotionObserver* observer : observers)
		if(observer->onMotion(event))
			return true;
	return false;
}
