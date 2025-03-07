#include "Settings.hpp"
#include "CursorManager.hpp"
#include "OpenGLContext.hpp"
#include "DrawingQueue.hpp"
#include "ShaderVariable.hpp"
#include "IApplication.hpp"
#include "KeyObserver.hpp"
#include "TickObserver.hpp"
#include "forcefield/ForceFieldOp.hpp"
#include "showobj/ShowObjOp.hpp"


class Application : public IApplication, KeyObserver, TickObserver
{
public:
	Application(int argc, char **argv);

	virtual void onGlContextChanged() override;

	virtual void onTick(real dt) override;

	virtual bool onKey(const KeyEvent& event) override;

	virtual void onDraw(int width, int height) override;

private:
	Settings settings;
	OpenGLContext renderer;
	Vec3D eyePos = Vec3D(0.0, 0.0, 7.0);

	DynamicArray<IVisual*> rootVisuals;
};


IApplication *IApplication::create(int argc, char **argv)
{
	CursorManager::getInstance();
	return new Application(argc, argv);
}


Application::Application(int /*argc*/, char ** /*argv*/)
	: settings("settings.json")
{
	rootVisuals.push_back(new ForceFieldOp(settings));
	//rootVisuals.push_back(new ShowObjOp());
}


void Application::onGlContextChanged()
{
	renderer.contextChanged();
}


bool Application::onKey(const KeyEvent& event)
{
	if(event.keyCode == KeyEvent::KEYCODE_BACK)
		exit(0);

	return false;
}


void Application::onTick(real /*dt*/)
{
}


void Application::onDraw(int width, int height)
{
	DrawingQueue drawingQueue;

	drawingQueue.pushMatrix(Matrix4({}, -eyePos));
	for(IVisual* vis : rootVisuals)
		vis->draw(drawingQueue);
	drawingQueue.popMatrix();

	renderer.render(width, height, eyePos, drawingQueue);
}
