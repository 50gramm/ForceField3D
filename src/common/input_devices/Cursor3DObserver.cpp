#include "Cursor3DObserver.hpp"
#include "Cursor3DObserver.hpp"


Cursor3DObserver::Cursor3DObserver()
{
	Cursor3DSubject::attach(this);
}


Cursor3DObserver::~Cursor3DObserver()
{
	Cursor3DSubject::detach(this);
}



DynamicArray<Cursor3DObserver*> Cursor3DSubject::observers;


void Cursor3DSubject::attach(Cursor3DObserver* observer)
{
	observers.add(observer);
}


void Cursor3DSubject::detach(Cursor3DObserver* observer)
{
	observers.remove_all(observer);
}


bool Cursor3DSubject::notify(const Cursor3DEvent& event)
{
	for(Cursor3DObserver* observer : observers)
		if(observer->onCursor3D(event))
			return true;
	return false;
}
