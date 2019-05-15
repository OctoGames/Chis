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
	score_(0.0f)
{
}

Canvas::~Canvas()
{
}

void Canvas::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_c"); if (it != params.end()) enabled_ = params.at("enabled_c").b;
	it = params.find("scheme"); if (it != params.end()) defaultScheme_ = params.at("scheme").s;
	it = params.find("cursor"); if (it != params.end()) defaultCursor_ = params.at("cursor").s;
	it = params.find("menu_layout"); if (it != params.end()) mainmenuLayout_ = params.at("menu_layout").s;
	it = params.find("game_layout"); if (it != params.end()) gameLayout_ = params.at("game_layout").s;
	it = params.find("end_layout"); if (it != params.end()) endmenuLayout_ = params.at("end_layout").s;
}

Component * Canvas::clone()
{
	Canvas* clonedComponent = new Canvas();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->defaultScheme_ = this->defaultScheme_;
	clonedComponent->defaultCursor_ = this->defaultCursor_;
	clonedComponent->mainmenuLayout_ = this->mainmenuLayout_;
	clonedComponent->gameLayout_ = this->gameLayout_;
	clonedComponent->endmenuLayout_ = this->endmenuLayout_;

	return clonedComponent;
}

void Canvas::init()
{
	// Set default GUI resources
	CEGUI::SchemeManager::getSingleton().createFromFile(defaultScheme_);
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
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
}

void Canvas::update()
{
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
		roots_[GUIContext::GAME]->getChild("ReticleWatergun")->show();
	}

	return true;
}

bool Canvas::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	GUIManager::Instance()->mouseReleased(e, id);

	return true;
}

void Canvas::changeReticle()
{
	if (roots_[currentGUIContext_]->getActiveChild() == roots_[currentGUIContext_]->getChild("ReticleWatergun"))
	{
		roots_[currentGUIContext_]->getChild("Reticle")->show();
		roots_[currentGUIContext_]->getChild("ReticleShotgun")->hide();
		roots_[currentGUIContext_]->getChild("ReticleWatergun")->hide();
	}
	else if (roots_[currentGUIContext_]->getActiveChild() == roots_[currentGUIContext_]->getChild("Reticle")
	{
		roots_[currentGUIContext_]->getChild("Reticle")->hide();
		roots_[currentGUIContext_]->getChild("ReticleShotgun")->show();
		roots_[currentGUIContext_]->getChild("ReticleWatergun")->hide();
	}
	else if (roots_[currentGUIContext_]->getActiveChild() == roots_[currentGUIContext_]->getChild("ReticleShotgun")
	{
		roots_[currentGUIContext_]->getChild("Reticle")->hide();
		roots_[currentGUIContext_]->getChild("ReticleShotgun")->hide();
		roots_[currentGUIContext_]->getChild("ReticleWatergun")->show();
	}
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
	roots_[currentGUIContext_]->getChild("ReticleWatergun")->hide();

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

void Canvas::updateHealth(float health)
{
	if (health <= 0) toEndMenu();
	std::string s = std::to_string(static_cast<int>(health));
	roots_[GUIContext::GAME]->getChild("Health")->setText("HEALTH: " + s);
}

void Canvas::updateScore(float score)
{
	score_ += score;
	std::string s = std::to_string(static_cast<int>(score_));
	roots_[GUIContext::GAME]->getChild("Score")->setText("SCORE: " + s);
}

void Canvas::updateCheese(float cheesyness)
{
	if (cheesyness < 75) roots_[GUIContext::GAME]->getChild("Life4")->hide();
	if (cheesyness < 50) roots_[GUIContext::GAME]->getChild("Life3")->hide();
	if (cheesyness < 25) roots_[GUIContext::GAME]->getChild("Life2")->hide();
	if (cheesyness <= 0) toEndMenu();
}
