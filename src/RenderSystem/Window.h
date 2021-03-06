#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_syswm.h>

#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

class Window
{
public:
	Window(Ogre::String windowName = "Ogre Application");
	~Window();

	inline SDL_Window* getNativeWindow() { return nativeWindow_; }
	inline Ogre::RenderWindow* getRenderWindow() { return renderWindow_; }

	void update();

private:
	SDL_Window* nativeWindow_;
	Ogre::RenderWindow* renderWindow_;
};

#endif // !__WINDOW_H__

