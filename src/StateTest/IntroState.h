#ifndef _INTRO_STATE_H_
#define _INTRO_STATE_H_

#include <Ogre.h>

#include "GameState.h"

class IntroState : public GameState
{
public:
	void enter();
	void exit();

	void pause();
	void resume();

	void keyClicked(Ogre::KeyEvent* e);
	void keyPressed(Ogre::KeyEvent* e);
	void keyReleased(Ogre::KeyEvent* e);
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	static IntroState* getInstance() { return &mIntroState; }
protected:
	IntroState() { }

	Ogre::Root *mRoot;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Viewport* mViewport;
	Ogre::InputReader* mInputDevice;
	Ogre::Camera* mCamera;
	bool mExitGame;
private:
	static IntroState mIntroState;
};

#endif // !_INTRO_STATE_H_