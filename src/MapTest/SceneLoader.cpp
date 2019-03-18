#include "SceneLoader.h"
//#include "gameobject.h"

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

	//// Process userDataReference (?)
	//pElement = XMLRoot->first_node("user_data");
	//if (pElement)
	//	processUserData(pElement, mAttachNode->getUserObjectBindings());

	//// Process terrain (?)
	//pElement = XMLRoot->first_node("terrain");
	//if (pElement)
	//	processTerrain(pElement);
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
	// Construct the node's name
	Ogre::String gameObjectName = getAttrib(XMLNode, "name");

	//creo el gameobject con el nombre, id, etc 

	// lista de componentes del gameobject
	std::list<std::string*> componentsList = LoadArchetypes::Instance()->getComponentsList(gameObjectName);

	// bucle recorriendo la lista de componentes, se llama a una funcion que se le pasa el nodo y donde está el switch tocho
		//en cada opcion del switch se crea el componente y se leen del xml los datos necesarios

	rapidxml::xml_node<>* pElement;

	//------------TRANFORM------------//
	// Process position (?)
	pElement = XMLNode->first_node("position");
	if (pElement)
	{
		//pedir el componente tranform con los parámetros leidos
		parseVector3(pElement);
	}

	// Process rotation (?)
	pElement = XMLNode->first_node("rotation");
	if (pElement)
	{
		//pedir el componente tranform con los parámetros leidos
		parseQuaternion(pElement);
	}

	// Process scale (?)
	pElement = XMLNode->first_node("scale");
	if (pElement)
	{
		//pedir el componente tranform con los parámetros leidos
		parseVector3(pElement);
	}
	//------------TRANFORM------------//



	// Process entity (*)
	pElement = XMLNode->first_node("entity");
	while (pElement)
	{
		processEntity(pElement);
		pElement = pElement->next_sibling("entity");
	}
}

void SceneLoader::processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String meshFile = getAttrib(XMLNode, "meshFile");
	bool castShadows = getAttribBool(XMLNode, "ghost", true);

	// TEMP: Maintain a list of static and dynamic objects
	
	rapidxml::xml_node<>* pElement;

	// Create the entity
	//Ogre::Entity *pEntity = 0;
	//try
	//{
		//Ogre::MeshManager::getSingleton().load(meshFile, m_sGroupName);
		/*pEntity = mSceneMgr->createEntity(name, meshFile);
		pEntity->setCastShadows(castShadows);
		pParent->attachObject(pEntity);*/

	//}
	//catch (Ogre::Exception &/*e*/)
	//{
	//	Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error loading an entity!");
	//}
}