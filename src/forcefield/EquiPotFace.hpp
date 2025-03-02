#pragma once
#include <memory>
#include "Array.hpp"
#include "Object.hpp"
#include "KeyObserver.hpp"
#include "ForceFieldState.hpp"
#include "SurfaceMeshing.hpp"
#include "VisualSettings.hpp"


class EquiPotFace : public IVisual, public KeyObserver
{
	const ForceFieldState& state;
	SurfaceMeshing surfaceMeshing;
	real faceRes = 0.0201732846;
	ForceFieldState::UniqueId generatedStateId = 0;

	bool show = true;

	void regenerate();
	
public:
	EquiPotFace(const ForceFieldState& state, const VisualSettings& visSettings);

	virtual void draw(DrawingQueue& drawing) override;

	virtual bool onKey(const KeyEvent& event) override;
};
