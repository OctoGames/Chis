#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>

#include "Component.h"

class InputManager : public OIS::KeyListener, public OIS::MouseListener
{
public:
	static InputManager* Instance();

	void init();
	void update(float deltaTime);
	void close();

	void addKeyListener(Component *keyListener, const std::string& instanceName);
	void addMouseListener(Component *mouseListener, const std::string& instanceName);
	void removeKeyListener(const std::string& instanceName);
	void removeMouseListener(const std::string& instanceName);
	void removeKeyListener(Component *keyListener);
	void removeMouseListener(Component *mouseListener);
	void removeAllListeners();
	void removeAllKeyListeners();
	void removeAllMouseListeners();

	inline OIS::Mouse* getMouse() { return mouse_; }
	inline OIS::Keyboard* getKeyboard() { return keyboard_; }

private:
	InputManager();

	// OIS::KeyListener
	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);

	// OIS::MouseListener
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	static InputManager* instance_;

	OIS::Mouse* mouse_;
	OIS::Keyboard* keyboard_;
	OIS::InputManager* inputManager_;

	std::map<std::string, Component*> keyListeners_;
	std::map<std::string, Component*> mouseListeners_;
};


#endif // !__INPUT_MANAGER_H__

