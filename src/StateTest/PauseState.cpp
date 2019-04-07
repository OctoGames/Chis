#include "PauseState.h"
#include "PlayState.h"

#include <Ogre.h>
#include <OgreKeyEvent.h>

using namespace Ogre;

PauseState PauseState::mPauseState;

void PauseState::enter()
{
	mInputDevice = InputManager::getSingletonPtr()->getInputDevice();
	mRoot = Root::getSingletonPtr();

	mViewport = mRoot->getAutoCreatedWindow()->getViewport(0);
	mViewport->setBackgroundColour(ColourValue(0.0, 1.0, 0.0));
}

void PauseState::exit()
{
}

void PauseState::pause()
{
}

void PauseState::resume()
{
}

void PauseState::keyClicked(KeyEvent* e)
{
}

void PauseState::keyPressed(KeyEvent* e)
{
	if (e->getKey() == KC_P)
	{
		popState();
	}
}

void PauseState::keyReleased(KeyEvent* e)
{
}

bool PauseState::frameStarted(const FrameEvent& evt)
{
	return true;
}

bool PauseState::frameEnded(const FrameEvent& evt)
{
	return true;
}