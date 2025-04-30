#pragma once
#include "Object.hpp"
#include "KeyObserver.hpp"
#include "DrawCommand.hpp"
#include "ForceFieldState.hpp"
#include "VisualSettings.hpp"


class EquiPotFace : public IVisual, public KeyObserver
{
	const ForceFieldState& state;
	const VisualSettings& visSettings;
	DrawCommandGL drawCmd;
	ForceFieldState::UniqueId generatedStateId = 0;

	bool show = true;

	void regenerate();
	
public:
	EquiPotFace(const ForceFieldState& state, const VisualSettings& visSettings);

	virtual void draw(DrawingQueue& drawing) override;

	virtual bool onKey(const KeyEvent& event) override;
};
