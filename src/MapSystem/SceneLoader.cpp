#include "SceneLoader.h"
//#include "gameobject.h"
//#include <Transform.h>

SceneLoader::SceneLoader() : DotSceneLoader() {

}
SceneLoader::~SceneLoader() {

}

void SceneLoader::processScene(rapidxml::xml_node<>* XMLRoot)
{
	// Process the scene parameters
	Ogre::String version = getAttrib(XMLRoot, "formatVersion", "unknown");

	Ogre::String message = "[DotSceneLoader] Parsing dotScene file with version " + version;
	if (XMLRoot->first_attribute("ID"))
		message += ", id " + Ogre::String(XMLRoot->first_attribute("ID")->value());
	if (XMLRoot->first_attribute("sceneManager"))
		message += ", scene manager " + Ogre::String(XMLRoot->first_attribute("sceneManager")->value());
	if (XMLRoot->first_attribute("minOgreVersion"))
		message += ", min. Ogre version " + Ogre::String(XMLRoot->first_attribute("minOgreVersion")->value());
	if (XMLRoot->first_attribute("author"))
		message += ", author " + Ogre::String(XMLRoot->first_attribute("author")->value());

	Ogre::LogManager::getSingleton().logMessage(message);

	rapidxml::xml_node<>* pElement;

	// Process nodes (?)
	pElement = XMLRoot->first_node("nodes");
	if (pElement)
		processNodes(pElement);
}

void SceneLoader::processNodes(rapidxml::xml_node<>* XMLNode)
{
	rapidxml::xml_node<>* pElement;

	// Process node (*)
	pElement = XMLNode->first_node("node");
	while (pElement)
	{
		processNode(pElement);
		pElement = pElement->next_sibling("node");
	}
}

void SceneLoader::processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent) {

	rapidxml::xml_node<>* pElement;
	// Get the gameobject name
	Ogre::String gameObjectName = getAttrib(XMLNode, "name");
	Ogre::String gameObjectTag = "";

	//-----look for the gameobject tag-----//
	pElement = XMLNode->first_node("user_data");
	bool findTag = false;
	while (pElement && !findTag)
	{
		Ogre::String nameProp = getAttrib(pElement, "name");
		Ogre::String typeProp = getAttrib(pElement, "type");
		if (nameProp == "tag" || nameProp == "Tag" || nameProp == "TAG" && typeProp == "str") {
			gameObjectTag = getAttrib(pElement, "value");
			findTag = true;
		}
		pElement = pElement->next_sibling("user_data");
	}
	//-----look for the gameobject tag-----//

	// create a gameobjects with its name and tag 
	GameObject* g = new GameObject(gameObjectName, gameObjectTag);

	// lista de componentes del gameobject
	std::list<std::string*> componentsList = LoadArchetypes::Instance()->getComponentsList(gameObjectName);

	// create components from the list of components with its parametres
	loadComponents(componentsList, XMLNode, g);
}

void SceneLoader::loadComponents(std::list<std::string*> componentsList, rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
{
	for (std::string* c : componentsList)
	{
		if (*c == "audiosource") {
			loadAudioSource(XMLNode, gameObject);
		}
		else if (*c == "camera") {
			//???
		}
		else if (*c == "light") {
			loadLight(XMLNode, gameObject);
		}
		else if (*c == "meshrenderer") {
			loadMeshRenderer(XMLNode, gameObject);
		}

		else if (*c == "rigidbody") {

			loadRigidBody(XMLNode, gameObject);
		}

		else if (*c == "transform") {
			loadTranform(XMLNode, gameObject);
		}
	}
}

void SceneLoader::loadAudioSource(rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
{
}

void SceneLoader::loadLight(rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
{
}

void SceneLoader::loadMeshRenderer(rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
{
	rapidxml::xml_node<>* pElement;
	//Read the entity node and find meshFile attribute
	Ogre::String meshFile = "";
	pElement = XMLNode->first_node("entity");
	if (pElement) meshFile = getAttrib(pElement, "meshFile");

	MeshRenderer* renderer = new MeshRenderer(gameObject, meshFile);
	//creo que no hace falta registrar el material, viene automáticamente con el mesh
	//renderer->setMaterialName(gameObject->getName() + ".material");
}

void SceneLoader::loadRigidBody(rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
{
	rapidxml::xml_node<>* pElement;
	RigidBody* rigidBody = new RigidBody(gameObject);

	// Find mass and rigid body type attributes
	pElement = XMLNode->first_node("user_data");
	Ogre::String radioSphereRB, massValue;
	double mass = 1.0;
	bool findRigidBody = false, findMass = false;
	
	while (pElement && (!findRigidBody || !findMass))
	{
		Ogre::String nameProp = getAttrib(pElement, "name");
		Ogre::String typeProp = getAttrib(pElement, "type");
		if (!findRigidBody && (nameProp == "SphereRB" || nameProp == "sphererb" || nameProp == "SPHERERB" && typeProp == "float")) {
			radioSphereRB = getAttrib(pElement, "value");
			findRigidBody = true;
		}

		else if (!findMass && (nameProp == "Mass" || nameProp == "mass" || nameProp == "MASS" && typeProp == "float")) {
			massValue = getAttrib(pElement, "value");
			mass = (double)Ogre::StringConverter::parseReal(massValue);
			findMass = true;
		}
		pElement = pElement->next_sibling("user_data");
	}

	if (findRigidBody) {
		rigidBody->createSphereRB(mass, (double)Ogre::StringConverter::parseReal(radioSphereRB), gameObject->getName() + "RB");
	}
	else {
		pElement = XMLNode->first_node("scale");
		rigidBody->createBoxRB(mass, parseVector3(pElement), gameObject->getName() + "RB");

	}
}

void SceneLoader::loadTranform(rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
{
	rapidxml::xml_node<>* pElement;
	Transform* transform = new Transform(gameObject);

	// Process position (?)
	pElement = XMLNode->first_node("position");
	if (pElement) transform->setPosition(parseVector3(pElement));

	// Process rotation (?)
	pElement = XMLNode->first_node("rotation");
	if (pElement) transform->setOrientation(parseQuaternion(pElement));

	// Process scale (?)
	pElement = XMLNode->first_node("scale");
	if (pElement) transform->setScale(parseVector3(pElement));
}



//void SceneLoader::processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
//{
//	// Process attributes
//	Ogre::String name = getAttrib(XMLNode, "name");
//	Ogre::String meshFile = getAttrib(XMLNode, "meshFile");
//	bool castShadows = getAttribBool(XMLNode, "ghost", true);
//
//	// TEMP: Maintain a list of static and dynamic objects
//
//	rapidxml::xml_node<>* pElement;
//
//	// Create the entity
//	//Ogre::Entity *pEntity = 0;
//	//try
//	//{
//		//Ogre::MeshManager::getSingleton().load(meshFile, m_sGroupName);
//		/*pEntity = mSceneMgr->createEntity(name, meshFile);
//		pEntity->setCastShadows(castShadows);
//		pParent->attachObject(pEntity);*/
//
//		//}
//		//catch (Ogre::Exception &/*e*/)
//		//{
//		//	Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error loading an entity!");
//		//}
//}