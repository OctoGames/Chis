#include "Canvas.h"

#include "Physics.h"
#include "EntityComponentManager.h"

std::string Canvas::name_ = "Canvas";

Canvas::Canvas() :
	currentGUIContext_(GUIContext::MAIN_MENU),
	defaultScheme_("Chis.scheme"),
	defaultCursor_("Chis/MouseArrow"),
	mainmenuLayout_("MainMenu.layout"),
	gameLayout_("Game.layout"),
	endmenuLayout_("EndMenu.layout")
{
}

Canvas::~Canvas()
{
}

void Canvas::load(const std::map<std::string, ValueType>& params)
{
	return;
}

Component * Canvas::clone()
{
	Canvas* clonedComponent = new Canvas();

	clonedComponent->enabled_ = this->enabled_;

	return clonedComponent;
}

void Canvas::init()
{
	// Set default GUI resources
	CEGUI::SchemeManager::getSingleton().createFromFile(defaultScheme_);
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(defaultCursor_);

	// Create a root for every game state
	roots_.push_back(GUIManager::Instance()->createRootWidget(mainmenuLayout_));
	roots_.push_back(GUIManager::Instance()->createRootWidget(gameLayout_));
	roots_.push_back(GUIManager::Instance()->createRootWidget(endmenuLayout_));

	// Init all GUI widgets for all game states
	GUIManager::Instance()->getCurrentRoot()->getChild("StartButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Canvas::toGame, this));
	GUIManager::Instance()->getCurrentRoot()->getChild("QuitButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Canvas::quit, this));

	InputManager::Instance()->addKeyListener(this, "Canvas");
	InputManager::Instance()->addMouseListener(this, "Canvas");

	setEnabled(enabled_);
}

void Canvas::start()
{
}

bool Canvas::keyPressed(const OIS::KeyEvent & e)
{
	GUIManager::Instance()->keyPressed(e);

#ifdef _DEBUG
	if (e.key == OIS::KC_O) Physics::Instance()->toggleDebugMode();
	else if (e.key == OIS::KC_P) Physics::Instance()->toggleDebug();
	else if (e.key == OIS::KC_F) RenderManager::Instance()->getSceneManager()->setFog(Ogre::FOG_EXP2, Ogre::ColourValue::White, 0.001);
#endif

	if (e.key == OIS::KC_ESCAPE)
	{
		if (GUIManager::Instance()->getContext().getMouseCursor().isVisible()) RenderManager::Instance()->setRunning(false);
		else toMainMenu();
	}

	return true;
}

bool Canvas::keyReleased(const OIS::KeyEvent & e)
{
	GUIManager::Instance()->keyReleased(e);

	return true;
}

bool Canvas::mouseMoved(const OIS::MouseEvent & e)
{
	GUIManager::Instance()->mouseMoved(e);

	return true;
}

bool Canvas::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	GUIManager::Instance()->mousePressed(e, id);

	return true;
}

bool Canvas::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	GUIManager::Instance()->mouseReleased(e, id);

	return true;
}

void Canvas::quit()
{
	RenderManager::Instance()->setRunning(false);
}

void Canvas::toGame()
{
	roots_[GUIContext::GAME]->show();
	roots_[GUIContext::END_MENU]->hide();
	roots_[GUIContext::MAIN_MENU]->hide();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(false);

	GUIManager::Instance()->setRootWidget(roots_[GUIContext::GAME]);

	SceneManager* sceneManager_ = static_cast<SceneManager*>(EntityComponentManager::Instance()->getComponent(gameObject(), "SceneManager"));
	sceneManager_->createGameScene();
}

void Canvas::toMainMenu()
{
	roots_[GUIContext::GAME]->hide();
	roots_[GUIContext::END_MENU]->hide();
	roots_[GUIContext::MAIN_MENU]->show();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(true);

	GUIManager::Instance()->setRootWidget(roots_[GUIContext::MAIN_MENU]);

	SceneManager* sceneManager_ = static_cast<SceneManager*>(EntityComponentManager::Instance()->getComponent(gameObject(), "SceneManager"));
	sceneManager_->createMenuScene();
}

void Canvas::toEndMenu()
{
	roots_[GUIContext::GAME]->hide();
	roots_[GUIContext::END_MENU]->show();
	roots_[GUIContext::MAIN_MENU]->hide();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(true);

	GUIManager::Instance()->setRootWidget(roots_[GUIContext::END_MENU]);

	// Call to SceneManager to change scene setup
}