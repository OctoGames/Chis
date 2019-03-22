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
	OgreSystem::Instance()->init();
}

void Application::closeSystems()
{
	OgreSystem::Instance()->close();
}

void Application::initDemo()
{
	// I create a camera. It represent a 'point of view' in the scene.
	Ogre::Camera* lCamera = OgreSystem::Instance()->getSceneManager()->createCamera("MyFirstCamera");

	// I attach the camera to a new SceneNode. It will be easier then to move it in the scene.
	cameraNode_ = OgreSystem::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("MyFirstCameraNode");
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
	viewport_ = OgreSystem::Instance()->getWindow()->addViewport(lCamera, lMainViewportZOrder, lViewportLeft, lViewportTop, lViewportWidth, lViewportHeight);

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
	OgreSystem::Instance()->getWindow()->setActive(true);

	// I want to update myself the content of the window, not automatically.
	OgreSystem::Instance()->getWindow()->setAutoUpdated(false);

	Ogre::Light* light_ = OgreSystem::Instance()->getSceneManager()->createLight("mainLight");
	light_->setType(Ogre::Light::LT_DIRECTIONAL);
	light_->setDiffuseColour(0.75, 0.75, 0.75);
	cameraNode_->attachObject(light_);

	Ogre::String lNameOfTheMesh = "mouse.mesh";
	int lNumberOfEntities = 5;
	for (int iter = 0; iter < lNumberOfEntities; ++iter)
	{
		Ogre::Entity* lEntity = OgreSystem::Instance()->getSceneManager()->createEntity(lNameOfTheMesh);
		// Now I attach it to a scenenode, so that it becomes present in the scene.
		Ogre::SceneNode* lNode = OgreSystem::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		lNode->attachObject(lEntity);
		// I move the SceneNode so that it is visible to the camera.
		float lPositionOffset = float(1 + iter * 2) - (float(lNumberOfEntities));
		lPositionOffset = lPositionOffset * 20;
		lNode->translate(lPositionOffset, lPositionOffset, -200.0f);
		lNode->setScale(30.0f, 30.0f, 30.0f);
		lEntity->setMaterialName("mouse_mat");

		Physics::Instance()->createBoxRigidBody(lNode, 0, Ogre::Vector3(30, 30, 30), "ch");
		Physics::Instance()->setDebugMode(true);
		// The loaded mesh will be white. This is normal.
	}
}

void Application::run()
{
	OgreSystem::Instance()->getRoot()->clearEventTimes();

	// The root has an access to Ogre3D's internal timer.
	// This timer can be controlled (for example, if you want to synchronise
	// some shaders between 5 computers), and accessed for time informations.
	// Here I reset the timer.
	Ogre::Timer* lOgreTimer = OgreSystem::Instance()->getRoot()->getTimer();
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
	while (!OgreSystem::Instance()->getWindow()->isClosed() && running_)
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
	// I capture the keyboard settings.
	// Then I update the scene according to these informations.
	OIS::Keyboard* lKeyboard = OgreSystem::Instance()->getKeyboard();
	OIS::Mouse* lMouse = OgreSystem::Instance()->getMouse();

	// The current time is used in the calculation : this is 'real time'. 
	// The camera move with the same speed on any computer.
	// I put a coefficient 200.0 because the scene is big.
	// I test the keys TGFH for moving.
	lKeyboard->capture();
	{
		float lCoeff = 200.0f * lDeltaTime_s;
		Ogre::Vector3 lTranslation(Ogre::Vector3::ZERO);
		if (lKeyboard->isKeyDown(OIS::KC_W))
		{
			lTranslation.z -= lCoeff;
		}
		if (lKeyboard->isKeyDown(OIS::KC_S))
		{
			lTranslation.z += lCoeff;
		}
		if (lKeyboard->isKeyDown(OIS::KC_A))
		{
			lTranslation.x -= lCoeff;
		}
		if (lKeyboard->isKeyDown(OIS::KC_D))
		{
			lTranslation.x += lCoeff;
		}
		if (lTranslation != Ogre::Vector3::ZERO)
		{
			cameraNode_->translate(lTranslation, Ogre::Node::TS_LOCAL);
		}
		if (lKeyboard->isKeyDown(OIS::KC_ESCAPE))
		{
			running_ = false;
		}
	}

	// same for the mouse.
	lMouse->capture();
	{
		const OIS::MouseState& lMouseState = lMouse->getMouseState();
		if (lMouseState.buttonDown(OIS::MB_Left))
		{
			//// I change the colour of the background...
			//float red = Ogre::Math::RangeRandom(0.1f, 0.9f);
			//float green = Ogre::Math::RangeRandom(0.1f, 0.9f);
			//float blue = Ogre::Math::RangeRandom(0.1f, 0.9f);
			//viewport_->setBackgroundColour(Ogre::ColourValue(red, green, blue));
		}
		float lMouseX = float(lMouseState.X.rel) / float(OgreSystem::Instance()->getWindow()->getWidth());
		float lMouseY = float(lMouseState.Y.rel) / float(OgreSystem::Instance()->getWindow()->getHeight());
		float lRotCoeff = -5.0f;
		Ogre::Radian lAngleX(lMouseX * lRotCoeff);
		Ogre::Radian lAngleY(lMouseY * lRotCoeff);

		// If the 'player' don't make loopings, 'yaw in world' + 'pitch in local' is often enough for a camera controler.
		cameraNode_->yaw(lAngleX, Ogre::Node::TS_WORLD);
		cameraNode_->pitch(lAngleY, Ogre::Node::TS_LOCAL);
	}
}

void Application::render()
{
	// the window update its content.
	// each viewport that is 'autoupdated' will be redrawn now,
	// in order given by its z-order.
	OgreSystem::Instance()->getWindow()->update(false);

	// The drawn surface is then shown on the screen
	// (google "double buffering" if you want more details).
	// I always use vertical synchro.
	bool lVerticalSynchro = true;
	OgreSystem::Instance()->getWindow()->swapBuffers();

	//// I print some statistics (more are available in Ogre3D)
	//// With VSync enabled, that should be close to 59,9fps.
	//const Ogre::RenderTarget::FrameStats& lStats = OgreSystem::Instance()->getWindow()->getStatistics();
	//std::cout << "FPS: " << lStats.lastFPS << "; AvgFPS : " << lStats.avgFPS;
	//std::cout << "; batchcount :" << lStats.batchCount << std::endl;

	// This update some internal counters and listeners.
	// Each render surface (window/rtt/mrt) that is 'auto-updated' has got its 'update' function called.
	OgreSystem::Instance()->getRoot()->renderOneFrame();

	//Ogre::WindowEventUtilities::messagePump();
}