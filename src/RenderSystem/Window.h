#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_syswm.h>

#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

class Window : public Ogre::WindowEventListener
{
public:
	Window(Ogre::String windowName = "Ogre Application");
	~Window();

	inline SDL_Window* getNativeWindow() { return nativeWindow_; }
	inline Ogre::RenderWindow* getRenderWindow() { return renderWindow_; }

	void update();

private:
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	SDL_Window* nativeWindow_;
	Ogre::RenderWindow* renderWindow_;
};

#endif // !__WINDOW_H__

