#include "Application.h"

Application::Application() :
	running_(true)
{
	initSystems();
	initDemo();
}

Application::~Application()
{
	closeSystems();
}

void Application::initSystems()
{
	RenderManager::Instance()->init();
}

void Application::closeSystems()
{
	RenderManager::Instance()->close();
}

void Application::initDemo()
{
	// I create a camera. It represent a 'point of view' in the scene.
	Ogre::Camera* lCamera = RenderManager::Instance()->getSceneManager()->createCamera("MyFirstCamera");

	// I attach the camera to a new SceneNode. It will be easier then to move it in the scene.
	cameraNode_ = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("MyFirstCameraNode");
	cameraNode_->attachObject(lCamera);

	// We create a viewport on a part of the window.
	// A viewport is the link between 1 camera and 1 drawing surface (here the window).
	// I can then call 'update();' on it to make it draw the Scene from the camera.
	// You can have several viewports on 1 window.
	// Check API for details on parameters.
	float lViewportWidth = 0.88f;
	float lViewportHeight = 0.88f;
	float lViewportLeft = (1.0f - lViewportWidth) * 0.5f;
	float lViewportTop = (1.0f - lViewportHeight) * 0.5f;
	unsigned short lMainViewportZOrder = 100;
	viewport_ = RenderManager::Instance()->getWindow()->addViewport(lCamera, lMainViewportZOrder, lViewportLeft, lViewportTop, lViewportWidth, lViewportHeight);

	// I want the viewport to draw the scene automatically
	// when I will call lWindow->update();
	viewport_->setAutoUpdated(true);

	// I choose a color for this viewport. 
	// I prefer to have a bright color, to detect holes in geometry etc...
	viewport_->setBackgroundColour(Ogre::ColourValue(1, 0, 1));

	// I choose the visual ratio of the camera. To make it looks real, I want it the same as the viewport.
	float ratio = float(viewport_->getActualWidth()) / float(viewport_->getActualHeight());
	lCamera->setAspectRatio(ratio);

	// I choose the clipping far& near planes. if far/near>2000, you can get z buffer problem.
	// eg : far/near = 10000/5 = 2000 . it's ok.
	// If (far/near)>2000 then you will likely get 'z fighting' issues.
	lCamera->setNearClipDistance(1.5f);
	lCamera->setFarClipDistance(3000.0f);

	// I want my window to be active
	RenderManager::Instance()->getWindow()->setActive(true);

	// I want to update myself the content of the window, not automatically.
	RenderManager::Instance()->getWindow()->setAutoUpdated(false);

	Ogre::Light* light_ = RenderManager::Instance()->getSceneManager()->createLight("mainLight");
	light_->setType(Ogre::Light::LT_DIRECTIONAL);
	light_->setDiffuseColour(0.75, 0.75, 0.75);
	cameraNode_->attachObject(light_);

	Ogre::String lNameOfTheMesh = "mouse.mesh";


	//------------------------------------------MOUSE 1----------------------------------------------
	Ogre::Entity* lEntity = RenderManager::Instance()->getSceneManager()->createEntity(lNameOfTheMesh);
	// Now I attach it to a scenenode, so that it becomes present in the scene.
	Ogre::SceneNode* lNode = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("nMouse" + std::to_string(1));
	lNode->attachObject(lEntity);
	
	// I move the SceneNode so that it is visible to the camera.
	lNode->translate(0, 100, -300.0f);
	lNode->setScale(30.0f, 30.0f, 30.0f);
	lEntity->setMaterialName("mouse_mat");

	Physics::Instance()->createBoxRigidBody(lNode, 10, Ogre::Vector3(30, 30, 30), "MouseRB" + std::to_string(1));
		

	//------------------------------------------MOUSE 2----------------------------------------------

	Ogre::Entity* lEntity2 = RenderManager::Instance()->getSceneManager()->createEntity(lNameOfTheMesh);
	// Now I attach it to a scenenode, so that it becomes present in the scene.
	Ogre::SceneNode* lNode2 = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("nFloor" + std::to_string(2));
	lNode2->attachObject(lEntity2);

	// I move the SceneNode so that it is visible to the camera.
	lNode2->translate(0, 0, -300.0f);
	lNode2->setScale(200, 5, 200);
	lEntity2->setMaterialName("mouse_mat");

	Physics::Instance()->createBoxRigidBody(lNode2, 0, Ogre::Vector3(200, 5, 200), "MouseRB" + std::to_string(2));


	//Physics::Instance()->setDebugMode(true);
}

void Application::run()
{
	RenderManager::Instance()->getRoot()->clearEventTimes();

	// The root has an access to Ogre3D's internal timer.
	// This timer can be controlled (for example, if you want to synchronise
	// some shaders between 5 computers), and accessed for time informations.
	// Here I reset the timer.
	Ogre::Timer* lOgreTimer = RenderManager::Instance()->getRoot()->getTimer();
	lOgreTimer->reset();

	// I name my variable with something that indicates milliseconds or seconds.
	// Here the _Ms means 'milliseconds',and '_s' means 'seconds'.
	unsigned long lLastTime_Ms = lOgreTimer->getMilliseconds();
	unsigned long lDeltaTime_Ms = 0;
	float lDeltaTime_s = 0.0f;

	// I wait until the window is closed.
	// The "message pump" thing is something you will see in most GUI application.
	// It allow the binding of messages between the application and the OS.
	// These messages are most of the time : keystroke, mouse moved, ... or window closed.
	// If I don't do this, the message are never caught, and the window won't close.
	while (!RenderManager::Instance()->getWindow()->isClosed() && running_)
	{
		// For the window drawing, you will increase performances if you : 
		// 0/ do some cpu calculations (ex: update sound).
		// 1/ clear the window buffer (Ogre::Viewport::clear)
		// 2/ do some cpu calculations (ex: particles)
		// 3/ draw the scene without swapping buffers (render() / update())
		// 4/ do some cpu calculations (ex : physics + logics)
		// 5/ swap the buffers(). (swapBuffers())
		// 6/ go back to 0/
		// Ogre allows to do that automatically if you use a framelistener,
		// like in the official ogre tutorials.
		// 
		// In this tutorial series, I don't want to bother you with that,
		// but you might keep that in mind when you design your final application.
		// Also it's better to launch sound first, because sound is slower than light.

		// I evaluate the current time and the time elapsed since last frame
		// I also prepare the next iteration.
		{
			unsigned long lCurrentTime_Ms = lOgreTimer->getMilliseconds();
			lDeltaTime_Ms = lCurrentTime_Ms - lLastTime_Ms;
			if (lDeltaTime_Ms == 0) continue;
			lLastTime_Ms = lCurrentTime_Ms;
			lDeltaTime_s = 0.001f * float(lDeltaTime_Ms);
		}

		handleInput(lDeltaTime_s);
		Physics::Instance()->update();
		render();
	}
}

void Application::handleInput(float lDeltaTime_s)
{

}

void Application::render()
{
	// the window update its content.
	// each viewport that is 'autoupdated' will be redrawn now,
	// in order given by its z-order.
	RenderManager::Instance()->getWindow()->update(false);

	// The drawn surface is then shown on the screen
	// (google "double buffering" if you want more details).
	// I always use vertical synchro.
	bool lVerticalSynchro = true;
	RenderManager::Instance()->getWindow()->swapBuffers();

	//// I print some statistics (more are available in Ogre3D)
	//// With VSync enabled, that should be close to 59,9fps.
	//const Ogre::RenderTarget::FrameStats& lStats = RenderManager::Instance()->getWindow()->getStatistics();
	//std::cout << "FPS: " << lStats.lastFPS << "; AvgFPS : " << lStats.avgFPS;
	//std::cout << "; batchcount :" << lStats.batchCount << std::endl;

	// This update some internal counters and listeners.
	// Each render surface (window/rtt/mrt) that is 'auto-updated' has got its 'update' function called.
	RenderManager::Instance()->getRoot()->renderOneFrame();

	//Ogre::WindowEventUtilities::messagePump();
}