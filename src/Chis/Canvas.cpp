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
	score_(0.0f),
	loadTimer_(nullptr),
	loadingMenu_(true)
{
}

Canvas::~Canvas()
{
}

void Canvas::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_cv"); if (it != params.end()) enabled_ = params.at("enabled_cv").b;
	it = params.find("scheme"); if (it != params.end()) defaultScheme_ = params.at("scheme").s;
	it = params.find("cursor"); if (it != params.end()) defaultCursor_ = params.at("cursor").s;
	it = params.find("menu_layout"); if (it != params.end()) mainmenuLayout_ = params.at("menu_layout").s;
	it = params.find("game_layout"); if (it != params.end()) gameLayout_ = params.at("game_layout").s;
	it = params.find("end_layout"); if (it != params.end()) endmenuLayout_ = params.at("end_layout").s;
	it = params.find("score_cv"); if (it != params.end()) score_ = params.at("score_cv").f;
	it = params.find("currentGUIContext"); if (it != params.end()) currentGUIContext_ = (GUIContext)(int)params.at("currentGUIContext").f;
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
	clonedComponent->score_ = this->score_;
	clonedComponent->currentGUIContext_= this->currentGUIContext_;

	return clonedComponent;
}

void Canvas::init()
{
	loadTimer_ = new Ogre::Timer();

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
	if (loadingMenu_ && loadTimer_->getMilliseconds() > 3000)
	{
		SceneManager* sceneManager_ = static_cast<SceneManager*>(EntityComponentManager::Instance()->getComponent(gameObject(), "SceneManager"));
		sceneManager_->createMenuScene();

		GUIManager::Instance()->getContext().getMouseCursor().setVisible(true);
		roots_[GUIContext::MAIN_MENU]->getChild("LoadingScreen")->hide();
		loadingMenu_ = false;
	}
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

	if (!loadingMenu_)
	{
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

void Canvas::changeReticle()
{
	if (roots_[currentGUIContext_]->getChild("Reticle")->isVisible())
	{
		roots_[currentGUIContext_]->getChild("Reticle")->hide();
		roots_[currentGUIContext_]->getChild("ReticleShotgun")->show();
		roots_[currentGUIContext_]->getChild("ReticleWatergun")->hide();
	}
	else if (roots_[currentGUIContext_]->getChild("ReticleShotgun")->isVisible())
	{
		roots_[currentGUIContext_]->getChild("Reticle")->hide();
		roots_[currentGUIContext_]->getChild("ReticleShotgun")->hide();
		roots_[currentGUIContext_]->getChild("ReticleWatergun")->show();
	}
	else if (roots_[currentGUIContext_]->getChild("ReticleWatergun")->isVisible())
	{
		roots_[currentGUIContext_]->getChild("Reticle")->show();
		roots_[currentGUIContext_]->getChild("ReticleShotgun")->hide();
		roots_[currentGUIContext_]->getChild("ReticleWatergun")->hide();
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
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(false);

	currentGUIContext_ = GUIContext::MAIN_MENU;
	GUIManager::Instance()->setRootWidget(roots_[currentGUIContext_]);
	roots_[GUIContext::MAIN_MENU]->getChild("Loading")->hide();
	loadTimer_->reset();
}

void Canvas::toGame()
{
	roots_[GUIContext::MAIN_MENU]->getChild("Loading")->show();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(false);
	RenderManager::Instance()->update(RenderManager::Instance()->time()->deltaTime());

	SceneManager* sceneManager_ = static_cast<SceneManager*>(EntityComponentManager::Instance()->getComponent(gameObject(), "SceneManager"));
	sceneManager_->createGameScene();

	roots_[GUIContext::GAME]->show();
	roots_[GUIContext::END_MENU]->hide();
	roots_[GUIContext::MAIN_MENU]->hide();
	currentGUIContext_ = GUIContext::GAME;
	GUIManager::Instance()->setRootWidget(roots_[currentGUIContext_]);
	roots_[currentGUIContext_]->getChild("Reticle")->show();
	roots_[currentGUIContext_]->getChild("ReticleShotgun")->hide();
	roots_[currentGUIContext_]->getChild("ReticleWatergun")->hide();
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
	roots_[GUIContext::GAME]->getChild("Health")->setText("Health: " + s);
}

void Canvas::updateScore(float score)
{
	score_ += score;
	std::string s = std::to_string(static_cast<int>(score_));
	roots_[GUIContext::GAME]->getChild("Score")->setText("Score: " + s);
}

void Canvas::updateCheese(float cheesyness)
{
	if (cheesyness < 75) roots_[GUIContext::GAME]->getChild("Life4")->hide();
	if (cheesyness < 50) roots_[GUIContext::GAME]->getChild("Life3")->hide();
	if (cheesyness < 25) roots_[GUIContext::GAME]->getChild("Life2")->hide();
	if (cheesyness <= 0) toEndMenu();
}

void Canvas::updateBullets(float remaining, float total)
{
	std::string r = std::to_string(static_cast<int>(remaining));
	std::string t = std::to_string(static_cast<int>(total));
	roots_[GUIContext::GAME]->getChild("Bullets")->setText("Bullets: " + r + "/" + t);
}
