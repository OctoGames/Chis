// Handle the game states and switch from one state to the other.
// This class, which is also a singleton, creates an InputManager and registers its EventProcessor.

#ifndef  _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <vector>
#include <Ogre.h>
#include <OgreEventListeners.h>
#include <OgreSingleton.h>

#include "InputManager.h"

class GameState;

class GameManager :
	public Ogre::FrameListener,
	public Ogre::KeyListener,
	public Ogre::Singleton<GameManager>
{
public:
	GameManager();
	~GameManager();
	void start(GameState* state);
	void changeState(GameState* state);
	void pushState(GameState* state);
	void popState();
	static GameManager& getSingleton(void);
	static GameManager* getSingletonPtr(void);

protected:
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mRenderWindow;
	InputManager* mInputManager;

	void setupResources(void);
	bool configure(void);

	void keyClicked(Ogre::KeyEvent* e);
	void keyPressed(Ogre::KeyEvent* e);
	void keyReleased(Ogre::KeyEvent* e);
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

private:
	std::vector<GameState*> mStates;
};

#endif // ! _GAME_MANAGER_H_