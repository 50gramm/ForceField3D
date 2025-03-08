#include "OpenGLDefs.hpp"
#include <GL/freeglut.h>
#include "Error.hpp"
#include "File.hpp"
#include "Settings.hpp"
#include "KeyObserver.hpp"
#include "MotionEvent.hpp"
#include "TickObserver.hpp"
#include "IApplication.hpp"


static IApplication *app = nullptr;
static int display_width, display_height;
static bool mouse_button_state[9] = {0};


static void shutdown(void)
{
	LOG_I("Shutdown");
	delete app;
	app = nullptr;
}


static void resized(int w, int h)
{
	display_width = w;
	display_height = h;
}


static void draw(void)
{
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	if(w != display_width || h != display_height)
		resized(w, h); // TODO

	app->onDraw(w, h);

	glutSwapBuffers();
}


static void mouse_move(int x, int y)
{
	MotionEvent event(display_width, display_height);
	event.action = MotionEvent::ACTION_DRAG;
	event.pointers.push_back(MotionEvent::Point(x, y));
	if(mouse_button_state[GLUT_RIGHT_BUTTON])
		event.pointers.push_back(MotionEvent::Point(x, y));
	MotionSubject::notify(event);
}


static void mouse_button(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		if(button == 3)
		{
			KeySubject::notify({KeyEvent::ACTION_DOWN, KeyEvent::KEYCODE_SCROLL_UP});
			return;
		}
		else if(button == 4)
		{
			KeySubject::notify({KeyEvent::ACTION_DOWN, KeyEvent::KEYCODE_SCROLL_DOWN});
			return;
		}
	}

	if(button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON)
	{
		MotionEvent event(display_width, display_height);
		event.action = (state == GLUT_DOWN ? MotionEvent::ACTION_DOWN : MotionEvent::ACTION_UP);
		event.pointers.push_back(MotionEvent::Point(x, y));
		if(button == GLUT_RIGHT_BUTTON)
			event.pointers.push_back(MotionEvent::Point(x, y));
		MotionSubject::notify(event);
		mouse_button_state[button] = (state == GLUT_DOWN);
	}
}


static void key_up(unsigned char key, int, int)
{
	int code = key;
	if(key == 27)
		code = KeyEvent::KEYCODE_BACK;

	KeySubject::notify({KeyEvent::ACTION_UP, code});
}


static void key_down(unsigned char key, int, int)
{
	int code = key;
	if(key == 27)
		code = KeyEvent::KEYCODE_BACK;

	KeySubject::notify({KeyEvent::ACTION_DOWN, code});
}


static void timer(int id)
{
	TickSubject::notify(0.03);

	glutPostRedisplay();
	glutTimerFunc(15, timer, id);
}


int main(int argc, char **argv)
{
	std::string fullPath = argv[0];
	int pos = std::max((int)fullPath.find_last_of('/'), (int)fullPath.find_last_of('\\'));
	std::string appDir;
	if(0 <= pos)
		appDir = fullPath.substr(0, pos);
	File::setFolder("{assets}", appDir + "/assets");
	File::setFolder("{user}", appDir);

	try {
		app = IApplication::create(argc, argv);
	} catch (const std::exception &e) {
		LOG_E("Exception occurred: %s", e.what());
	} catch (const std::string &msg) {
		LOG_E("Exception occurred: %s", msg.c_str());
	}

	Settings settings("{assets}/settings.json");
	display_width = settings["Visual"]["WindowSize"][0];
	display_height = settings["Visual"]["WindowSize"][1];

	// init GLUT and create Window
	glutInit(&argc, argv);
	unsigned displayMode = GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA;
	if(settings["Visual"]["Multisampling"])
		displayMode |= GLUT_MULTISAMPLE;
	if(0 < settings["Visual"]["SampleNum"])
		glutSetOption(GLUT_MULTISAMPLE, settings["Visual"]["SampleNum"]);
	glutInitDisplayMode(displayMode);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(display_width, display_height);
	glutCreateWindow(MODULE_NAME);
	if(settings["Visual"]["Fullscreen"])
		glutFullScreen();

	gladLoadGL();

	app->onGlContextChanged();
	app->onStart();

	// register callbacks
	glutMouseFunc(mouse_button);
	glutMotionFunc(mouse_move);
	glutPassiveMotionFunc(mouse_move);
	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);
	glutDisplayFunc(draw);
	//glutReshapeFunc(resized);
	glutCloseFunc(shutdown);
	
	timer(0);

	try {
		// run main loop
		glutMainLoop();
	} catch(const std::string& str) {
		LOG_E("Exception occurred: %s", str.c_str());
	} catch(const char* str) {
		LOG_E("Exception occurred: %s", str);
	}
	
	LOG_I("Finish");
	return 0;
}
