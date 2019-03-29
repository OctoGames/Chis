#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <OIS/OISEvents.h>
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISJoystick.h>
#include <OIS/OISInputManager.h>

class InputManager : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
public:
	static InputManager* Instance();

	void init();
	void update(float deltaTime);
	void close();

	void addKeyListener(OIS::KeyListener *keyListener, const std::string& instanceName);
	void addMouseListener(OIS::MouseListener *mouseListener, const std::string& instanceName);
	void addJoystickListener(OIS::JoyStickListener *joystickListener, const std::string& instanceName);

	void removeKeyListener(const std::string& instanceName);
	void removeMouseListener(const std::string& instanceName);
	void removeJoystickListener(const std::string& instanceName);

	void removeKeyListener(OIS::KeyListener *keyListener);
	void removeMouseListener(OIS::MouseListener *mouseListener);
	void removeJoystickListener(OIS::JoyStickListener *joystickListener);

	void removeAllListeners();
	void removeAllKeyListeners();
	void removeAllMouseListeners();
	void removeAllJoystickListeners();

	void setWindowExtents(int width, int height);

	inline OIS::Mouse* getMouse() { return mouse_; }
	inline OIS::Keyboard* getKeyboard() { return keyboard_; }
	inline OIS::JoyStick* getJoystick(unsigned int index) { if (index < getNumOfJoysticks()) return joysticks_[index]; }
	inline int getNumOfJoysticks() const { return joysticks_.size(); }

private:
	InputManager();

	// OIS::KeyListener
	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);

	// OIS::MouseListener
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	// OIS::JoystickListener
	virtual bool povMoved(const OIS::JoyStickEvent &e, int pov);
	virtual bool axisMoved(const OIS::JoyStickEvent &e, int axis);
	virtual bool sliderMoved(const OIS::JoyStickEvent &e, int sliderID);
	virtual bool buttonPressed(const OIS::JoyStickEvent &e, int button);
	virtual bool buttonReleased(const OIS::JoyStickEvent &e, int button);

	static InputManager* instance_;

	OIS::Mouse* mouse_;
	OIS::Keyboard* keyboard_;
	OIS::InputManager* inputManager_;
	std::vector<OIS::JoyStick*> joysticks_;

	std::map<std::string, OIS::KeyListener*> keyListeners_;
	std::map<std::string, OIS::MouseListener*> mouseListeners_;
	std::map<std::string, OIS::JoyStickListener*> joystickListeners_;
};


#endif // !__INPUT_MANAGER_H__

