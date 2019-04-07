#include "InputManager.h"

#include <OgreEventProcessor.h>

template<> InputManager* Ogre::Singleton<InputManager>::ms_Singleton = 0;

InputManager::InputManager(Ogre::RenderWindow* rwindow)
{
	mEventProcessor = new Ogre::EventProcessor();
	mEventProcessor->initialise(rwindow);
	mEventProcessor->startProcessingEvents();
	mInputDevice = mEventProcessor->getInputReader();
}

InputManager::~InputManager()
{
	if (mEventProcessor)
		delete mEventProcessor;

	assert(mInputDevice);
	Ogre::PlatformManager::getSingleton().destroyInputReader(mInputDevice);
}