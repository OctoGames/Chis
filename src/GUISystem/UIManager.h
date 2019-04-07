#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include <OgreFrameListener.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "InputManager.h"
#include "RenderManager.h"

class UIManager : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	static UIManager* Instance();

	void init();
	void close();

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	bool isMenuClosed() const { return menuClosed_; }

	void quit();
	void closeMenu();

	void openMenu();

private:
	UIManager();

	static UIManager* instance_;

	CEGUI::OgreRenderer* renderer_;

	CEGUI::FrameWindow* fWnd;

	bool menuClosed_;
};

#endif // !__UI_MANAGER_H__
