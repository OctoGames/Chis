#ifndef _OGRE_SYSTEM_H
#define _OGRE_SYSTEM_H

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>

class OgreSystem
{
public:
	static OgreSystem* Instance();

	inline OIS::Mouse* getMouse() { return mouse_; }
	inline OIS::Keyboard* getKeyboard() { return keyboard_; }
	inline OIS::JoyStick* getJoystick() { return joystick_; }

private:
	OgreSystem();

	static OgreSystem* instance_;

	OIS::Mouse* mouse_;
	OIS::Keyboard* keyboard_;
	OIS::JoyStick* joystick_;
	OIS::InputManager* inputManager_;
};

#endif // !_OGRE_SYSTEM_H
