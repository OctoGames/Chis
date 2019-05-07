#ifndef __GUI_MANAGER_H__
#define __GUI_MANAGER_H__

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "InputManager.h"
#include "RenderManager.h"

class GUIManager
{
public:
	static GUIManager* Instance();

	void init();
	void update(float deltaTime);
	void close();

	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);
	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	CEGUI::GUIContext& getContext() { return CEGUI::System::getSingleton().getDefaultGUIContext(); }
	inline CEGUI::OgreRenderer* getOgreRenderer() { return renderer_; }
	inline CEGUI::Window* getCurrentRoot() { return currentRoot_; }

	CEGUI::Window* createWidget(const CEGUI::String& widgetType, const CEGUI::String& widgetName, CEGUI::Window* parent = nullptr);
	CEGUI::Window* createRootWidget(const CEGUI::String& rootName);
	void setRootWidget(CEGUI::Window* root);

private:
	GUIManager();

	static GUIManager* instance_;

	CEGUI::Window* currentRoot_;
	CEGUI::OgreRenderer* renderer_;
};

#endif // !__GUI_MANAGER_H__
