#include "PlayState.h"
#include "IntroState.h"
#include "PauseState.h"

#include <Ogre.h>
#include <OgreKeyEvent.h>

using namespace Ogre;

PlayState PlayState::mPlayState;

void PlayState::enter()
{
	mInputDevice = InputManager::getSingletonPtr()->getInputDevice();
	mRoot = Root::getSingletonPtr();

	//should be for Ogre 1.2 createSceneManager(ST_GENERIC);
	mSceneMgr = mRoot->getSceneManager(ST_GENERIC);
	mCamera = mSceneMgr->createCamera("IntroCamera");
	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue(0.0, 0.0, 1.0));
}

void PlayState::exit()
{
	mSceneMgr->clearScene();
	//!! Note: This is supposed to be mSceneMgr->destroyAllCameras(); for CVS head
	mSceneMgr->removeAllCameras();
	mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause()
{
}

void PlayState::resume()
{
	mViewport->setBackgroundColour(ColourValue(0.0, 0.0, 1.0));
}

void PlayState::keyClicked(KeyEvent* e)
{
}

void PlayState::keyPressed(KeyEvent* e)
{
	if (e->getKey() == KC_P)
	{
		pushState(PauseState::getInstance());
	}

	if (e->getKey() == KC_ESCAPE)
	{
		changeState(IntroState::getInstance());
	}
}

void PlayState::keyReleased(KeyEvent* e)
{
}

bool PlayState::frameStarted(const FrameEvent& evt)
{
	return true;
}

bool PlayState::frameEnded(const FrameEvent& evt)
{
	return true;
}