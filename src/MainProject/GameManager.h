#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "Component.h"
#include "GUIManager.h"

enum GUIContext { MAIN_MENU, GAME, END_MENU };

class GameManager : public Component
{
public:
	GameManager();
	virtual ~GameManager();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void start();

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

private:
	// Scene functions
	void createMenuScene();
	void createGameScene();

	// GUI building functions
	void createMainMenuGUI();
	void createGameGUI();
	void createEndMenuGUI();

	// Button callbacks
	void quit();
	void toGame();
	void toMainMenu();
	void toEndMenu();

	static std::string name_;

	GUIContext currentGUIContext_;
	std::vector<CEGUI::Window*> roots_;

};

class GameManagerFactory : public BaseFactory
{
public:
	GameManagerFactory() {}
	virtual ~GameManagerFactory() {}

	virtual Component* create() { return new GameManager(); }
};

#endif // !__GAME_MANAGER_H__
