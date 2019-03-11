#include "Application.h"


Application::Application(std::string appName) : appName_(appName), mRoot(0), fullScreen_(false), winWidth_(800), winHeight_(600)
{
#if _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	//mRoot = new Ogre::Root(mPluginsCfg);
	mFSLayer = new Ogre::FileSystemLayer(appName);

	//Load all needed plugins and initialize the root
	loadPluggins();

	readDataFromFile(); //reads all the data needed to create the window

	//Create the window
	setWindow();

	//SceneManager
	sManager = mRoot->createSceneManager();

	//SetUp and Load all resources from config file if there's no problem on load
	setUpResources();
	loadResources();

	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	unsigned long hWnd = 0;

	//Get Ogre window attributes
	mWindow->getCustomAttribute("WINDOW", &hWnd);

	//Create a SDL window based on thos attributes to detect input
	SDL_CreateWindowFrom((void*)hWnd);
	
	//After we create the SDL window, we set the Ogre window mode to fullscreen if required to solve blackscreen bug
	if (fullScreen_)
		mWindow->setFullscreen(true, winWidth_, winHeight_);


	//We create here an entity to check everything is ok (It also initialized the camera)
	createEntity();

	//Init Lights
	initLights();
}

Application::~Application()
{
	//delete mRoot;

	//delete mWindow;

	//delete sManager;

	//delete viewport;

	//delete mainCamera;
}

void Application::render()
{
	mRoot->renderOneFrame();
}

void Application::update()
{
	physic->frameStarted();
}

//Load all needed plugins and initialize the root
void Application::loadPluggins()
{
	Ogre::String pluginsPath;
	pluginsPath = mFSLayer->getConfigFilePath("plugins.cfg");

	mRoot = new Ogre::Root(pluginsPath);
}

void Application::setWindow()
{
	//Only one available render system on our proyect
	mRoot->setRenderSystem(*(mRoot->getAvailableRenderers().begin()));
	mRoot->initialise(false);

	//Window
	mWindow = mRoot->createRenderWindow(appName_, winWidth_, winHeight_, false);

	mWindow->setActive(true);
	mWindow->setAutoUpdated(true);
	mWindow->setDeactivateOnFocusChange(false);
}

//Initialize the main camera being a child of the main character
void Application::initCamera()
{
	// viewport and camera
	mainCamera = sManager->createCamera("mainCamera");
	mainCamera->setNearClipDistance(1);
	mainCamera->setFarClipDistance(10000);
	mainCamera->setAutoAspectRatio(true);

	mCamNode = mainCharacter->getMainCharacterNode()->createChildSceneNode("ncam");   //sManager->getRootSceneNode()->getChild("mainCharacter")->createChild("nCam");
	//mCamNode = sManager->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(mainCamera);

	//mCamNode->setPosition(0, 0, 0);
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	viewport = mWindow->addViewport(mainCamera);

}

void Application::initLights()
{
	// without light we would just get a black screen 
	Ogre::Light* luz = sManager->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(0.75, 0.75, 0.75);

	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);
	mLightNode->setDirection(Ogre::Vector3(0, -1, -1));
}

//Reads the data from a .config file given to setUp the screen
void Application::readDataFromFile()
{

	std::string configFilePath = mFSLayer->getConfigFilePath("plugins.cfg");
	configFilePath.erase(configFilePath.find_last_of("\\") + 1, configFilePath.size() - 1);

	//Read the information to FullScreen
	std::string line;
	std::string fullScreen;

	line = findConfig("FULLSCREEN", configFilePath);

	fullScreen = readString(line);

	if (fullScreen == "TRUE")
		fullScreen_ = true;
	
	//Read data config to screen width
	line = findConfig("WIDTH", configFilePath);
	std::string auxString = readString(line);
	winWidth_ = std::stoi(auxString);

	//Read data config to screen height
	line = findConfig("HEIGHT", configFilePath);
	auxString = readString(line);
	winHeight_ = std::stoi(auxString);
}

std::string Application::findConfig(std::string config, std::string configFilePath)
{
	std::ifstream configFile;

	configFile.open(configFilePath + "Configuration.config");

	std::string auxString;
	std::string line;
	bool found = false;

	while (!configFile.eof() && !found)
	{
		//read data to fullScreen Mode
		std::getline(configFile, line);

		auxString = line;

		if (auxString.erase(auxString.find_last_of("="), auxString.size() - 1) == config)
		{
			found = true;
		}

	}

	configFile.close();

	return line;
}

//Read the string until find = sing. Then takes the whole right part and returns it
std::string Application::readString(std::string s)
{
	int i = 0;
	bool found = false;
	std::string auxString;

	while (i < s.size() && !found)
	{
		if (s[i] == '=')
		{
			found = true;
			auxString = s.substr(i + 1, s.size() - 1);
		}
		else i++;
	}

	return auxString;
}

//this method parses the resources.cfg file how? not 100% sure but it works fine
void Application::setUpResources()
{
	resourcesPath = mFSLayer->getConfigFilePath("resources.cfg");
	Ogre::ConfigFile cf;

	if (Ogre::FileSystemLayer::fileExists(resourcesPath))
	{	
		cf.load(resourcesPath);
	}

	Ogre::String sec, type, arch;
	// go through all specified resource groups
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci)
	{
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// go through all resource paths
		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;

			//Added the whole route to fix the problem with other computers route
			std::string auxPath = resourcesPath;
			//We take the resources path and delete the finel path
			auxPath.erase(auxPath.find_last_of("\\") + 1, auxPath.size() - 1);

			//then, to taht last path, we add the one where resources will be located
			arch = auxPath + Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);

	OgreAssert(!genLocs.empty(), ("Resource Group '" + sec + "' must contain at least one entry").c_str());

	arch = genLocs.front().archive->getName();
	type = genLocs.front().archive->getType();
}

void Application::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

//Here we create a simple entity to see everything above works fine
void Application::createEntity()
{
	mainCharacter = new MainCharacter(sManager);

	//Create Ground
	Ogre::MeshManager::getSingleton().createPlane("GroundPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 10000, 10000, 100, 80, true, 1, 10.0, 10.0, (Ogre::Vector3::NEGATIVE_UNIT_Z));

	Ogre::Entity* ground_ent = sManager->createEntity("GroundPlane");
	ground_ent->setMaterialName("ground_mat");

	mGroundNode = sManager->getRootSceneNode()->createChildSceneNode("nGround");
	mGroundNode->attachObject(ground_ent);




	physic = new Physics();

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));

	btScalar groundMass(0.); //the mass is 0, because the ground is immovable (static)
	btVector3 localGroundInertia(0, 0, 0);

	btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(groundTransform);

	groundShape->calculateLocalInertia(groundMass, localGroundInertia);

	btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
	btRigidBody *groundBody = new btRigidBody(groundRBInfo);

	//add the body to the dynamics world
	physic->addToPhysicWorld(groundBody);





	sManager->setSkyPlane(true, Ogre::Plane(Ogre::Vector3::UNIT_Z, -50),
		"skyPlane", 1, 1, true, 1.0, 100, 100);

	initCamera();

	Ogre::SceneNode* gunNode = mCamNode->createChildSceneNode("gun");

	Ogre::Entity* gunent = sManager->createEntity("fish.mesh");

	gunNode->attachObject(gunent);

	gunNode->setPosition(30, -20, -100);
	gunNode->setScale(10, 10, 10);
	gunNode->yaw(Ogre::Degree(-90));
	gunNode->setVisible(true);


	Ogre::SceneNode* mouseNode = sManager->getRootSceneNode()->createChildSceneNode("mouse");

	Ogre::Entity* mouseent = sManager->createEntity("mouse.mesh");
	mouseent->setMaterialName("mouse_mat");

	mouseNode->attachObject(mouseent);

	mouseNode->setPosition(0, 100, 0);
	mouseNode->setScale(30, 30, 30);
	//mouseNode->yaw(Ogre::Degree(-90));
	mouseNode->setVisible(true);

	




	//create the new shape, and tell the physics that is a Box
	btCollisionShape *newRigidShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	physic->getCollisionShapes().push_back(newRigidShape);

	//set the initial position and transform. For this demo, we set the tranform to be none
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));

	//set the mass of the object. a mass of "0" means that it is an immovable object
	btScalar mass = 0.1f;
	btVector3 localInertia(0, 0, 0);

	

	startTransform.setOrigin(btVector3(0, 100, 0));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//actually contruvc the body and add it to the dynamics world
	btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	body->setRestitution(1);
	body->setUserPointer(mouseNode);

	physic->addToPhysicWorld(body);
	physic->trackRigidBodyWithName(body, "mouse");

}




//This method will handle the input from SDL and return the event taken
SDL_Event Application::handleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_w)
			{
				mainCharacter->getMainCharacterNode()->translate(0, 0, -50, Ogre::Node::TS_LOCAL);
				std::cout << "Moving Forward" << std::endl;
			}

			else if (event.key.keysym.sym == SDLK_s)
			{
				mainCharacter->getMainCharacterNode()->translate(0, 0, 50, Ogre::Node::TS_LOCAL);
				std::cout << "Moving Backwards" << std::endl;
			}

			else if (event.key.keysym.sym == SDLK_a)
			{
				mainCharacter->getMainCharacterNode()->translate(-50, 0, 0, Ogre::Node::TS_LOCAL);
				std::cout << "Moving Left" << std::endl;
			}

			else if (event.key.keysym.sym == SDLK_d)
			{
				mainCharacter->getMainCharacterNode()->translate(50, 0, 0, Ogre::Node::TS_LOCAL);
				std::cout << "Moving Right" << std::endl;
			}

			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				event.type = SDL_QUIT;
				
			}

			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				physic->getRigidBodyByName("mouse")->applyCentralImpulse(btVector3(0, 50, 0));
			}
		}
	}

	return event;

}