#include "Window.h"

#include "RenderManager.h"
//#include "InputManager.h"

Window::Window(Ogre::String windowName) :
	nativeWindow_(nullptr),
	renderWindow_(nullptr)
{
	int w, h, flags;
	Ogre::String token;
	Ogre::ConfigOptionMap ropts;
	Ogre::NameValuePairList miscParams;

	ropts = RenderManager::Instance()->getRoot()->getRenderSystem()->getConfigOptions();
	miscParams["FSAA"] = ropts["FSAA"].currentValue;
	miscParams["vsync"] = ropts["VSync"].currentValue;
	miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;
	std::istringstream mode(ropts["Video Mode"].currentValue); mode >> w >> token >> h;
	if (ropts["Full Screen"].currentValue == "Yes") flags = SDL_WINDOW_FULLSCREEN;
	else flags = SDL_WINDOW_RESIZABLE;

	SDL_SysWMinfo wmInfo;
	if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_InitSubSystem(SDL_INIT_VIDEO);
	nativeWindow_ = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
	SDL_VERSION(&wmInfo.version); SDL_GetWindowWMInfo(nativeWindow_, &wmInfo);
	miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
	
	SDL_WarpMouseInWindow(nativeWindow_, SDL_GetWindowSurface(nativeWindow_)->w / 2, SDL_GetWindowSurface(nativeWindow_)->h / 2);
	SDL_SetWindowBordered(nativeWindow_, SDL_bool(false));
	SDL_SetWindowGrab(nativeWindow_, SDL_bool(false));
	SDL_ShowCursor(false);

	renderWindow_ = RenderManager::Instance()->getRoot()->createRenderWindow(windowName, w, h, flags, &miscParams);
	Ogre::WindowEventUtilities::addWindowEventListener(renderWindow_, this);
}

Window::~Window()
{
	renderWindow_->removeAllViewports();
	renderWindow_->destroy();
	renderWindow_ = nullptr;
	SDL_DestroyWindow(nativeWindow_);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	nativeWindow_ = nullptr;
}

void Window::update()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			windowClosed(renderWindow_);
			break;

		case SDL_WINDOWEVENT_RESIZED:
			windowResized(renderWindow_);
			break;
		}
	}

	Ogre::WindowEventUtilities::messagePump();
}

void Window::windowResized(Ogre::RenderWindow * rw)
{
	if (rw == renderWindow_)
	{
		int left, top;
		unsigned int width, height, depth;
		rw->getMetrics(width, height, depth, left, top);
		//if (InputManager::Instance()->getMouse()) InputManager::Instance()->setWindowExtents(width, height);
	}
}

void Window::windowClosed(Ogre::RenderWindow * rw)
{
	if (rw == renderWindow_)
	{
		RenderManager::Instance()->setRunning(false);
	}
}