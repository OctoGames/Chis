#include "Window.h"

Window::Window() :
	nativeWindow_(nullptr),
	renderWindow_(nullptr)
{
}

Window::~Window()
{
	delete renderWindow_;
	renderWindow_ = nullptr;
	SDL_DestroyWindow(nativeWindow_);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	nativeWindow_ = nullptr;
}

void Window::createNativeWindow(const std::string& windowName, int width, int height, int flags)
{
	if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_InitSubSystem(SDL_INIT_VIDEO);
	nativeWindow_ = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
}

void Window::setWindowGrab(bool grab, bool cursor)
{
	SDL_SetWindowGrab(nativeWindow_, SDL_bool(grab));
	SDL_ShowCursor(cursor);
}