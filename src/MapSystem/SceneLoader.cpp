#include "SceneLoader.h"

SceneLoader* SceneLoader::instance_ = nullptr;

SceneLoader::SceneLoader()
{
}

SceneLoader* SceneLoader::Instance()
{
	if (instance_ == nullptr) {
		instance_ = new SceneLoader();
	}
	return instance_;
}

void SceneLoader::loadScene(const Ogre::String& SceneName)
{
	Ogre::DataStreamPtr stream = Ogre::Root::openFileStream(PATH_ + SceneName);

	rapidxml::xml_document<> XMLDoc;    // character type defaults to char
	rapidxml::xml_node<>* XMLRoot;

	char* scene = _strdup(stream->getAsString().c_str());
	XMLDoc.parse<0>(scene);

	// Grab the scene node
	XMLRoot = XMLDoc.first_node("scene");

	// Validate the File
	if (getAttrib(XMLRoot, "formatVersion", "") == "")
	{
		Ogre::LogManager::getSingleton().logMessage("[SceneLoader] Error: Invalid .scene File structure. Missing <scene>");
		delete scene;
		return;
	}

	Ogre::String messageError = "";
	// Process the scene
	processScene(XMLRoot, messageError);

	Ogre::LogManager::getSingleton().logMessage(messageError);


	delete scene;
}

//Reads each "nodes" field and the scale they'll have
void SceneLoader::processScene(rapidxml::xml_node<>* XMLRoot, Ogre::String &messageError)
{
	bool processControl = false;

	rapidxml::xml_node<>* pElement;

	pElement = XMLRoot->first_node("nodes");
	factor_escala = getAttribReal(pElement, "factor");
	if (pElement)
		processGameObjects(pElement, messageError);
}

//Reads each field of the node
void SceneLoader::processGameObjects(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError)
{
	rapidxml::xml_node<>* pElement;

	// Process node (*)
	pElement = XMLNode->first_node("node");
	while (pElement)
	{
		processGameObject(pElement, messageError);
		pElement = pElement->next_sibling("node");
	}
}

//Reads the Game Object's position, scale and orientation and clones it
void SceneLoader::processGameObject(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError) {

	// Get the gameobject name
	Ogre::String gameObjectName = getAttrib(XMLNode, "name", "");
	Ogre::StringUtil::toLowerCase(gameObjectName);

	if (gameObjectName == "")
		messageError = messageError + " [SceneLoader] Error: Incorrect loading of the game object name \n";
	else {
		processComponentsData(XMLNode, messageError);

		Prototype* prototype = EntityComponentManager::Instance()->getPrototype(params_.at("prototype_name").s);
		GameObject* clonedObject = prototype->getEntity()->clone();
		clonedObject->setName(gameObjectName);

		clonedObject->transform()->setPosition(params_.at("pos_x").f * factor_escala, params_.at("pos_y").f * factor_escala, params_.at("pos_z").f * factor_escala);
		clonedObject->transform()->setScale(params_.at("scale_x").f*factor_escala, params_.at("scale_y").f * factor_escala, params_.at("scale_z").f * factor_escala);
		clonedObject->transform()->setOrientation(params_.at("rot_w").f, params_.at("rot_x").f, params_.at("rot_y").f, params_.at("rot_z").f);
		
		for (Component* c : prototype->getComponents()) {
			Component* clonedComponent = c->clone();
			clonedComponent->load(params_);
			clonedComponent->setContainer(clonedObject);
			clonedComponent->init();
		}
	}
}

//Reads the information of the Game Object's components in each "user_data" field
void SceneLoader::processComponentsData(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError) {

	rapidxml::xml_node<>* pElement;
	pElement = XMLNode->first_node("user_data");
	params_.clear();
	while (pElement)
	{
		processDataParametres(pElement, messageError);

		pElement = pElement->next_sibling("user_data");
	}

}