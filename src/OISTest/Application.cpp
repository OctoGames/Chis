#include "Application.h"

Application::Application() :
	fpsCamera_(nullptr)
{
	RenderManager::Instance()->init();
	InputManager::Instance()->init();
	createScene();
}

Application::~Application()
{
	InputManager::Instance()->close();
	RenderManager::Instance()->close();
}

void Application::run()
{
	while (RenderManager::Instance()->isRunning())
	{
		float dt = RenderManager::Instance()->time()->tick();
		if (dt == 0.0f) continue;

		// AudioSystem::Instance()->update();
		InputManager::Instance()->update(dt);
		updateScene(dt);
		// PhysicsSystem::Instance()->update();
		// EntityComponentManager::Instance()->update();
		RenderManager::Instance()->update(dt);
	}
}

void Application::createScene()
{
	fpsCamera_ = new FirstPersonCamera();

	Ogre::String lNameOfTheMesh = "mouse.mesh";
	int lNumberOfEntities = 5;
	for (int iter = 0; iter < lNumberOfEntities; ++iter)
	{
		Ogre::Entity* lEntity = RenderManager::Instance()->getSceneManager()->createEntity(lNameOfTheMesh);
		Ogre::SceneNode* lNode = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		lNode->attachObject(lEntity);
		float lPositionOffset = float(1 + iter * 2) - (float(lNumberOfEntities));
		lPositionOffset = lPositionOffset * 20;
		lNode->translate(lPositionOffset, lPositionOffset, -200.0f);
		lNode->setScale(30.0f, 30.0f, 30.0f);
		lEntity->setMaterialName("mouse_mat");
	}
}

void Application::updateScene(float deltaTime)
{
	if (InputManager::Instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE))
	{
		RenderManager::Instance()->setRunning(false);
	}

	fpsCamera_->update(deltaTime);
}