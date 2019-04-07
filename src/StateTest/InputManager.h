// Its role is to create an EventProcessor and expose an InputReader to the game states,
// which can be used for both buffered and unbuffered input. Unbuffered input will be used
// in the game itself and the buffered one will be use for states which should be treated as menus.

#ifndef  _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <OgreSingleton.h>
#include <OgreInput.h>

class InputManager : 
	public Ogre::Singleton<InputManager>
{
public:
	InputManager(Ogre::RenderWindow* rwindow);
	virtual ~InputManager();

	inline Ogre::InputReader* getInputDevice() const { return mInputDevice; }
	inline Ogre::EventProcessor* getEventProcessor() const { return mEventProcessor; }

private:
	Ogre::EventProcessor* mEventProcessor;
	Ogre::InputReader* mInputDevice;
};

#endif // ! _INPUT_MANAGER_H_

// Input management for the game itself(unbuffered input) should be handled in the frameStarted and frameEnded functions,
// using the mInputDevice->isKeyDown() call like this :

// if (mInputDevice->isKeyDown(KC_RIGHT)) x++;
// if (mInputDevice->isKeyDown(KC_LEFT)) x--;
// if (mInputDevice->isKeyDown(KC_UP)) y++;
// if (mInputDevice->isKeyDown(KC_DOWN)) y--;