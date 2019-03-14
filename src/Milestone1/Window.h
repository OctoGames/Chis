#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <OgreRenderWindow.h>
#include <SDL.h>

class Window
{
public:
	Window();
	~Window();

	inline SDL_Window* getNativeWindow() { return nativeWindow_; }
	inline void setRenderWindow(Ogre::RenderWindow* renderWindow) { renderWindow_ = renderWindow; }

	void createNativeWindow(const std::string& windowName, int width, int height, int flags);
	void setWindowGrab(bool grab, bool cursor);

private:
	SDL_Window* nativeWindow_;
	Ogre::RenderWindow* renderWindow_;
};

#endif // !_WINDOW_H_
