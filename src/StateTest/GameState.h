// http://wiki.ogre3d.org/Managing+Game+States+with+OGRE
// http://wiki.ogre3d.org/Game+State+Manager


// This is a basic game state class, which you'll need to inherit from for all the game states you want to handle in your game

#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <Ogre.h>

class GameState
{
public:
	virtual void enter() = 0;
	virtual void exit() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	// Note that all the input code related to buffered input should be in the 
	// keyClicked, keyPressed and keyReleased functions, using the e->getKey() 
	// call as shown in the sources.This is needed for everything related to menus or GUI.

	virtual void keyClicked(Ogre::KeyEvent* e) = 0;
	virtual void keyPressed(Ogre::KeyEvent* e) = 0;
	virtual void keyReleased(Ogre::KeyEvent* e) = 0;
	virtual bool frameStarted(const Ogre::FrameEvent& evt) = 0;
	virtual bool frameEnded(const Ogre::FrameEvent& evt) = 0;

	void changeState(GameState* state) { GameManager::getSingletonPtr()->changeState(state); }
	void pushState(GameState* state) { GameManager::getSingletonPtr()->pushState(state); }
	void popState() { GameManager::getSingletonPtr()->popState(); }
	//protected:
protected:
	GameState() {}
};

#endif // !_GAME_STATE_H_