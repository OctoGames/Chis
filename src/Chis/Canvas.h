#ifndef __CANVAS_H__
#define __CANVAS_H__

#include <Engine.h>

enum GUIContext { MAIN_MENU, GAME, END_MENU };

class Canvas : public Component
{
public:
	Canvas();
	virtual ~Canvas();

	virtual void load(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }
	virtual Component* clone();
	virtual void init();

	virtual void start();
	virtual void update();

	virtual bool keyPressed(const OIS::KeyEvent &e);
	virtual bool keyReleased(const OIS::KeyEvent &e);
	virtual bool mouseMoved(const OIS::MouseEvent &e);
	virtual bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	virtual void changeReticle();

	void toMainMenu();

	void updateScore(float score);
	void updateHealth(float health);
	void updateCheese(float cheesyness);
	void updateBullets(float remaining, float total);

private:
	void quit();
	void toGame();
	void toEndMenu();
	void restart();

	static std::string name_;

	GUIContext currentGUIContext_;
	std::vector<CEGUI::Window*> roots_;

	std::string defaultScheme_;
	std::string defaultCursor_;
	std::string mainmenuLayout_;
	std::string gameLayout_;
	std::string endmenuLayout_;

	float score_;

	std::string currentReticle_;
};

class CanvasFactory : public BaseFactory
{
public:
	CanvasFactory() {}
	virtual ~CanvasFactory() {}

	virtual Component* create() { return new Canvas(); }
};

#endif // !__CANVAS_H__
