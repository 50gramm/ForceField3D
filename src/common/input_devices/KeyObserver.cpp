#include "KeyObserver.hpp"


KeyObserver::KeyObserver()
{
	KeySubject::attach(this);
}


KeyObserver::~KeyObserver()
{
	KeySubject::detach(this);
}



DynamicArray<KeyObserver*> KeySubject::observers;


void KeySubject::attach(KeyObserver* observer)
{
	observers.add(observer);
}


void KeySubject::detach(KeyObserver* observer)
{
	observers.remove_all(observer);
}


bool KeySubject::notify(const KeyEvent& event)
{
	for(KeyObserver* observer : observers)
		if(observer->onKey(event))
			return true;
	return false;
}
