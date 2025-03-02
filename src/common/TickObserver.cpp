#include "TickObserver.hpp"


TickObserver::TickObserver()
{
	TickSubject::attach(this);
}


TickObserver::~TickObserver()
{
	TickSubject::detach(this);
}



DynamicArray<TickObserver*> TickSubject::observers;


void TickSubject::attach(TickObserver* observer)
{
	observers.add(observer);
}


void TickSubject::detach(TickObserver* observer)
{
	observers.remove_all(observer);
}


void TickSubject::notify(real dt)
{
	for(TickObserver* observer : observers)
		observer->onTick(dt);
}
