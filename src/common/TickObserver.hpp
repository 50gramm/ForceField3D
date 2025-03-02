#pragma once
#include "Array.hpp"
#include "Core.hpp"


class TickObserver
{
public:
	TickObserver();
	~TickObserver();
	
	virtual void onTick(real dt) = 0;
};


class TickSubject
{
	static DynamicArray<TickObserver*> observers;

public:
	static void attach(TickObserver* observer);
	static void detach(TickObserver* observer);

	static void notify(real dt);
};
