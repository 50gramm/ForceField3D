#pragma once
#include "Object.hpp"
#include "MathUtils.hpp"
#include "KeyObserver.hpp"
#include "TickObserver.hpp"
#include "Cursor3DObserver.hpp"
#include "CursorManager.hpp"


class SmartMovingOp : public Cursor3DObserver, KeyObserver, TickObserver
{
	Object3D& obj;
	Vec3D prevCursorPos;
	Matrix4 prevCursorMat;
	bool moving = false;
	enum RotatinMode
	{
		NoRotating,
		ManualRotating,
		AutoRotating
	};
	RotatinMode rotating = NoRotating;
	Matrix4 rotAroundCursor;
	int rotIdleTick = 0;


	void doTranslation(const Matrix4& cursorMat)
	{
		obj.setPosition(cursorMat * prevCursorMat.inverse() * obj.getPosition());
	}


	void doRotation(const Matrix4& cursorMat)
	{
		Vec3D cursorPos = cursorMat.getTranslation();
		Vec3D moveVec = cursorPos - prevCursorPos;
		if(isNearZero(moveVec))
			return;
		
		Vec3D rotVec = moveVec % Vec3D(0,0,1);
		Matrix4 rotMat({rotVec, moveVec.len()*9}, Vec3D(0,0,0));
		Matrix4 transMat({}, cursorPos);
		rotAroundCursor = transMat * rotMat * transMat.inverse();
		obj.setPosition(rotAroundCursor * obj.getPosition());

		rotating = ManualRotating;
		rotIdleTick = 0;
	}

public:
	SmartMovingOp(Object3D& obj)
		: obj(obj)
	{}


	virtual bool onCursor3D(const Cursor3DEvent& event) override
	{
		bool res = false;

		if(event.type == Cursor3DEvent::EVENT_DOWN)
		{
			prevCursorPos = event.pos;
			prevCursorMat = event.mat;

			rotating = NoRotating;
			res = true;
			moving = true;
		}
		else if(event.type == Cursor3DEvent::EVENT_MOVE && moving)
		{
			rotating = NoRotating;
			if(CursorManager::getButtonState(0))
				doTranslation(event.mat);
			else if(CursorManager::getButtonState(2))
				doRotation(event.mat);
			res = true;
		}
		else if(event.type == Cursor3DEvent::EVENT_UP && moving)
		{
			res = true;
			moving = false;
			if(rotating == ManualRotating && rotIdleTick < 9)
				rotating = AutoRotating;
		}

		prevCursorPos = event.pos;
		prevCursorMat = event.mat;

		return res;
	}


	virtual void onTick(real /*dt*/) override
	{
		rotIdleTick += 1;
		if(rotating == AutoRotating)
		{
			obj.setPosition(rotAroundCursor * obj.getPosition());
		}
	}


	virtual bool onKey(const KeyEvent& event) override
	{
		if(event.action == KeyEvent::ACTION_DOWN)
		{
			if(event.keyCode == '+')
			{
				obj.setPosition(Matrix4({}, Vec3D(0,0,0), 0.99f) * obj.getPosition());
				return true;
			}
			else if(event.keyCode == '-')
			{
				obj.setPosition(Matrix4({}, Vec3D(0,0,0), 1.0f/0.99f) * obj.getPosition());
				return true;
			}
		}

		return false;
	}
};
