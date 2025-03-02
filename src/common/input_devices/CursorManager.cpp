#include "Error.hpp"
#include "CursorManager.hpp"


CursorManager& CursorManager::getInstance()
{
	static CursorManager instance;
	return instance;
}


bool CursorManager::onMotion(const MotionEvent& event)
{
	mat[0][3] =  event.pnorm().x;
	mat[1][3] = -event.pnorm().y;

	event3D.mat = mat;
	event3D.pos = mat.getTranslation();
	event3D.button = (event.pointers.size() == 1 ? 0 : 2);
	
	
	if(event.action == MotionEvent::ACTION_DRAG)
	{
		event3D.type = Cursor3DEvent::EVENT_MOVE;
	}
	else if(event.action == MotionEvent::ACTION_DOWN)
	{
		buttonStates[event3D.button] = true;
		event3D.type = Cursor3DEvent::EVENT_DOWN;
	}
	else if(event.action == MotionEvent::ACTION_UP)
	{
		buttonStates[event3D.button] = false;
		event3D.type = Cursor3DEvent::EVENT_UP;
	}

	if(1 < prevEvent.pointerNum() && event.pointerNum() <= 1)
	{
		buttonStates[2] = false;
	}

	prevEvent = event;

	Cursor3DSubject::notify(event3D);
	return true;
}


bool CursorManager::onKey(const KeyEvent& event)
{
	bool res = false;

	if(event.action == KeyEvent::ACTION_DOWN)
	{
		if(event.keyCode == KeyEvent::KEYCODE_SCROLL_DOWN)
		{
			mat[2][3] += 0.1;
			res = true;
		}
		else if(event.keyCode == KeyEvent::KEYCODE_SCROLL_UP)
		{
			mat[2][3] -= 0.1;
			res = true;
		}
	}

	if(res)
	{
		event3D.mat = mat;
		event3D.pos = mat.getTranslation();
		event3D.type = Cursor3DEvent::EVENT_MOVE;
		Cursor3DSubject::notify(event3D);
	}

	return res;
}


bool CursorManager::getButtonState(int button)
{
	if(!getInstance().buttonStates.count(button))
		return false;
	
	return getInstance().buttonStates.at(button);
}

