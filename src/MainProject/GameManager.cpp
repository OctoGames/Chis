#include "GameManager.h"

#include "Camera.h"
#include "AudioSource.h"
#include "Physics.h"
#include "SceneLoader.h"
#include "EntityComponentManager.h"

std::string GameManager::name_ = "GameManager";

// COMPONENT functions
GameManager::GameManager() :
	currentGUIContext_(GUIContext::MAIN_MENU)
{
}

GameManager::~GameManager()
{
}

void GameManager::load(const std::map<std::string, ValueType>& params)
{
	return;
}

Component * GameManager::clone()
{
	GameManager* clonedComponent = new GameManager();

	clonedComponent->enabled_ = this->enabled_;

	return clonedComponent;
}

void GameManager::init()
{
	// Set default GUI resources
	CEGUI::SchemeManager::getSingleton().createFromFile("Chis.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("Chis/MouseArrow");

	// Create a root for every game state
	roots_.push_back(GUIManager::Instance()->createRootWidget("MainMenu.layout"));
	roots_.push_back(GUIManager::Instance()->createRootWidget("Game.layout"));
	roots_.push_back(GUIManager::Instance()->createRootWidget("EndMenu.layout"));

	// Init all GUI widgets for all game states
	createMainMenuGUI();
	createGameGUI();
	createEndMenuGUI();

	InputManager::Instance()->addKeyListener(this, "GameManager");
	InputManager::Instance()->addMouseListener(this, "GameManager");

	setEnabled(enabled_);
}

void GameManager::start()
{
	createMenuScene();
}

bool GameManager::keyPressed(const OIS::KeyEvent & e)
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

bool GameManager::keyReleased(const OIS::KeyEvent & e)
{
	GUIManager::Instance()->keyReleased(e);

	return true;
}

bool GameManager::mouseMoved(const OIS::MouseEvent & e)
{
	GUIManager::Instance()->mouseMoved(e);

	return true;
}

bool GameManager::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	GUIManager::Instance()->mousePressed(e, id);

	return true;
}

bool GameManager::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	GUIManager::Instance()->mouseReleased(e, id);

	return true;
}


// SCENE MANAGER component functions
void GameManager::createMenuScene()
{
	RenderManager::Instance()->getSceneManager()->setSkyPlane(true, Ogre::Plane(Ogre::Vector3::UNIT_Z, -50), "Cheese", 1, 1, true, 1.0, 100, 100);
	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));
}

void GameManager::createGameScene()
{
	SceneLoader::Instance()->loadScene("Scene1.scene");
	RenderManager::Instance()->getSceneManager()->setSkyDome(true, "skyPlane");
	RenderManager::Instance()->getSceneManager()->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));

	// Play the main theme music
	AudioSource* song = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
	song->play();

// This goes in Physics
#if _DEBUG
	// If true the framerate drops too much
	Physics::Instance()->setDebugMode(false);
#else
	Physics::Instance()->setDebugMode(false);
#endif
}


// CANVAS component aux functions
void GameManager::createMainMenuGUI()
{
	GUIManager::Instance()->getCurrentRoot()->getChild("StartButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameManager::toGame, this));
	GUIManager::Instance()->getCurrentRoot()->getChild("QuitButton")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameManager::quit, this));
}

void GameManager::createGameGUI()
{
}

void GameManager::createEndMenuGUI()
{
}

void GameManager::quit()
{
	RenderManager::Instance()->setRunning(false);
}

void GameManager::toGame()
{
	roots_[GUIContext::GAME]->show();
	roots_[GUIContext::END_MENU]->hide();
	roots_[GUIContext::MAIN_MENU]->hide();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(false);

	GUIManager::Instance()->setRootWidget(roots_[GUIContext::GAME]);

	createGameScene();

	RenderManager::Instance()->getSceneManager()->setSkyPlaneEnabled(false);
}

void GameManager::toMainMenu()
{
	roots_[GUIContext::GAME]->hide();
	roots_[GUIContext::END_MENU]->hide();
	roots_[GUIContext::MAIN_MENU]->show();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(true);

	GUIManager::Instance()->setRootWidget(roots_[GUIContext::MAIN_MENU]);
}

void GameManager::toEndMenu()
{
	roots_[GUIContext::GAME]->hide();
	roots_[GUIContext::END_MENU]->show();
	roots_[GUIContext::MAIN_MENU]->hide();
	GUIManager::Instance()->getContext().getMouseCursor().setVisible(true);

	GUIManager::Instance()->setRootWidget(roots_[GUIContext::END_MENU]);
}
