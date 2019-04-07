#include "IntroState.h"
#include "PlayState.h"

#include <Ogre.h>
#include <OgreKeyEvent.h>

using namespace Ogre;

IntroState IntroState::mIntroState;

void IntroState::enter()
{
	mInputDevice = InputManager::getSingletonPtr()->getInputDevice();
	mRoot = Root::getSingletonPtr();

	//should be for Ogre 1.2 createSceneManager(ST_GENERIC);
	mSceneMgr = mRoot->getSceneManager(ST_GENERIC);
	mCamera = mSceneMgr->createCamera("IntroCamera");
	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue(1.0, 0.0, 0.0));

	mExitGame = false;
}

void IntroState::exit()
{
	mSceneMgr->clearScene();
	//!! Note: This is supposed to be mSceneMgr->destroyAllCameras(); for CVS head
	mSceneMgr->removeAllCameras();
	mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause()
{
}

void IntroState::resume()
{
}

void IntroState::keyClicked(KeyEvent* e)
{
}

void IntroState::keyPressed(KeyEvent* e)
{
	if (e->getKey() == KC_SPACE)
	{
		changeState(PlayState::getInstance());
	}

	if (e->getKey() == KC_ESCAPE)
	{
		mExitGame = true;
	}
}

void IntroState::keyReleased(KeyEvent* e)
{
}

bool IntroState::frameStarted(const FrameEvent& evt)
{
	return true;
}

bool IntroState::frameEnded(const FrameEvent& evt)
{
	if (mExitGame)
		return false;

	return true;
}