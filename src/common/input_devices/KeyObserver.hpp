#pragma once

#include "Array.hpp"


struct KeyEvent
{
	enum KeyAction
	{
		ACTION_DOWN,
		ACTION_UP
	} action;

	enum KeyCodes
	{
		KEYCODE_BACK = 0x10000,
		KEYCODE_CLOSE,
		KEYCODE_SCROLL_UP,
		KEYCODE_SCROLL_DOWN,
	};

	int keyCode;
};


class KeyObserver
{
public:
	KeyObserver();
	~KeyObserver();
	
	virtual bool onKey(const KeyEvent& event) = 0;
};


class KeySubject
{
	static DynamicArray<KeyObserver*> observers;

public:
	static void attach(KeyObserver* observer);
	static void detach(KeyObserver* observer);

	static bool notify(const KeyEvent& event);
};
