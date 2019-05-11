#include "Canvas.h"

#include "SceneManager.h"
#include "ChisApp.h"


std::string Canvas::name_ = "Canvas";

// COMPONENT FUNCTIONS --------------------------------------------------------
Canvas::Canvas() :
	currentGUIContext_(GUIContext::MAIN_MENU),
	defaultScheme_("Chis.scheme"),
	defaultCursor_("Chis/MouseArrow"),
	mainmenuLayout_("MainMenu.layout"),
	gameLayout_("Game.layout"),
	endmenuLayout_("EndMenu.layout"),
	lives_(4)
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
	roots_[GUIContext::MAIN_MENU]->getChild("StartButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Canvas::toGame, this));
	roots_[GUIContext::MAIN_MENU]->getChild("QuitButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Canvas::quit, this));
	roots_[GUIContext::END_MENU]->getChild("ReplayButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Canvas::restart, this));
	roots_[GUIContext::END_MENU]->getChild("QuitButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Canvas::quit, this));

	InputManager::Instance()->addKeyListener(this, "Canvas");
	InputManager::Instance()->addMouseListener(this, "Canvas");

	setEnabled(enabled_);
}

void Canvas::start()
{
	toMainMenu();
}


// INPUT FUNCTIONS ------------------------------------------------------------
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
		if (GUIManager::Instance()->getContext().getMouseCursor().isVisible()) quit();
		else toEndMenu();
	}
	else if (e.key == OIS::KC_NUMPADENTER)
	{
		if (currentGUIContext_ == GUIContext::MAIN_MENU) toGame();
		if (currentGUIContext_ == GUIContext::END_MENU) restart();
	}
	else if (e.key == OIS::KC_SPACE)
	{
		lives_--;
		hitPlayer();
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

	if (id == OIS::MB_Right)
	{
		roots_[GUIContext::GAME]->getChild("Reticle")->hide();
		roots_[GUIContext::GAME]->getChild("ReticleShotgun")->show();
	}

	return true;
}

bool Canvas::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	GUIManager::Instance()->mouseReleased(e, id);

	return true;
}


// BUTTON CALLBACKS -----------------------------------------------------------
void Canvas::quit()
{
	RenderManager::Instance()->setRunning(false);
}

void Canvas::toMainMenu()
{
	roots_[GUIContext::GAME]->hide();
	roots_[GUIContext::END_MENU]->hide();
	roots_[GUIContext::MAIN_MENU]->show();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(true);

	currentGUIContext_ = GUIContext::MAIN_MENU;
	GUIManager::Instance()->setRootWidget(roots_[currentGUIContext_]);

	SceneManager* sceneManager_ = static_cast<SceneManager*>(EntityComponentManager::Instance()->getComponent(gameObject(), "SceneManager"));
	sceneManager_->createMenuScene();
}

void Canvas::toGame()
{
	roots_[GUIContext::GAME]->show();
	roots_[GUIContext::END_MENU]->hide();
	roots_[GUIContext::MAIN_MENU]->hide();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(false);

	currentGUIContext_ = GUIContext::GAME;
	GUIManager::Instance()->setRootWidget(roots_[currentGUIContext_]);
	roots_[currentGUIContext_]->getChild("Reticle")->show();
	roots_[currentGUIContext_]->getChild("ReticleShotgun")->hide();

	SceneManager* sceneManager_ = static_cast<SceneManager*>(EntityComponentManager::Instance()->getComponent(gameObject(), "SceneManager"));
	sceneManager_->createGameScene();
}

void Canvas::toEndMenu()
{
	roots_[GUIContext::GAME]->hide();
	roots_[GUIContext::END_MENU]->show();
	roots_[GUIContext::MAIN_MENU]->hide();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(true);

	currentGUIContext_ = GUIContext::END_MENU;
	GUIManager::Instance()->setRootWidget(roots_[currentGUIContext_]);

	SceneManager* sceneManager_ = static_cast<SceneManager*>(EntityComponentManager::Instance()->getComponent(gameObject(), "SceneManager"));
	sceneManager_->createEndScene();
}

void Canvas::restart()
{
	ChisApp::reset_ = true;
	RenderManager::Instance()->setRunning(false);
}

void Canvas::hitPlayer()
{
	if (lives_ == 3) roots_[GUIContext::GAME]->getChild("Life4")->hide();
	else if (lives_ == 2) roots_[GUIContext::GAME]->getChild("Life3")->hide();
	else if (lives_ == 1) roots_[GUIContext::GAME]->getChild("Life2")->hide();
	else toEndMenu();
}
