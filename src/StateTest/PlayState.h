#ifndef _PLAY_STATE_H_
#define _PLAY_STATE_H_

#include <Ogre.h>

#include "GameState.h"

class PlayState : public GameState
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

	static PlayState* getInstance() { return &mPlayState; }
protected:
	PlayState() { }

	Ogre::Root *mRoot;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Viewport* mViewport;
	Ogre::InputReader* mInputDevice;
	Ogre::Camera* mCamera;
private:
	static PlayState mPlayState;
};

#endif // !_PLAY_STATE_H_