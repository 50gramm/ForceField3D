#pragma once


class IApplication
{
public:
	static IApplication *create(int argc, char **argv);

	virtual ~IApplication() {}

	virtual void onStart() {}

	virtual void onStop() {}

	virtual void onGlContextChanged() {}

	virtual void onDraw(int width, int height) = 0;
};
