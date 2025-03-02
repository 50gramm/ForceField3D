#pragma once

#include <map>
#include "TransformationMatrix.hpp"
#include "Cursor3DObserver.hpp"
#include "KeyObserver.hpp"
#include "MotionEvent.hpp"

class MotionEvent;


class CursorManager : public KeyObserver, MotionObserver
{
	Matrix4 mat;
	std::map<int,bool> buttonStates;
	MotionEvent prevEvent;
	Cursor3DEvent event3D;

	CursorManager()
		: mat(1)
		, prevEvent(0,0)
	{}

public:
	static CursorManager& getInstance();

	bool onMotion(const MotionEvent& event) override;

	bool onKey(const KeyEvent& event) override;

	static const Matrix4& getCursorPos() { return getInstance().mat; }

	static bool getButtonState(int button = 0);
};
