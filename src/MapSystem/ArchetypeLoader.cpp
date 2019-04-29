#include "ArchetypeLoader.h"

ArchetypeLoader* ArchetypeLoader::instance_ = nullptr;

ArchetypeLoader::ArchetypeLoader()
{
}

ArchetypeLoader* ArchetypeLoader::Instance()
{
	if (instance_ == nullptr) {
		instance_ = new ArchetypeLoader();
	}
	return instance_;
}

void ArchetypeLoader::loadArchetypes()
{
	Ogre::String messageError = "";

	for (const auto & entry : fs::directory_iterator(PATH_)) {
		std::string file_name = entry.path().filename().u8string();
		std::string extension = file_name.substr(file_name.length() - 4, file_name.length());
		Ogre::StringUtil::toLowerCase(extension);
		if (extension == "arch")
			readArchetypeFile(file_name, messageError);
		else
			messageError += "[ArchetypeLoader] Error: Invalid .arch File. Wrong extension: " + file_name + "\n";
	}

	Ogre::LogManager::getSingleton().logMessage(messageError);
}

void ArchetypeLoader::readArchetypeFile(const std::string& file, Ogre::String &messageError)
{
	Ogre::DataStreamPtr stream = Ogre::Root::openFileStream(PATH_ + file);

	rapidxml::xml_document<> XMLDoc;    // character type defaults to char
	rapidxml::xml_node<>* XMLRoot = nullptr;

	char* scene = _strdup(stream->getAsString().c_str());
	XMLDoc.parse<0>(scene);

	// Grab the scene node
	XMLRoot = XMLDoc.first_node("archetype");

	// Validate the File
	if (XMLRoot == nullptr)
	{
		messageError += "[ArchetypeLoader] Error: Invalid .arch File structure. Missing <archetypes> \n";
		delete scene;
		return;
	}

	// Process the scene
	processArchetypes(XMLRoot, messageError);

	delete scene;
}

void ArchetypeLoader::processArchetypes(rapidxml::xml_node<>* XMLRoot, Ogre::String &messageError)
{
	rapidxml::xml_node<>* pElement;

	// Process node (*)
	pElement = XMLRoot->first_node("entity");
	while (pElement)
	{
		processEntity(pElement, messageError);
		pElement = pElement->next_sibling("entity");
	}
}

void ArchetypeLoader::processEntity(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError) 
{
	// Get the gameobject name
	Ogre::String gameObjectName = getAttrib(XMLNode, "name", "invalid");
	Ogre::StringUtil::toLowerCase(gameObjectName);
	Ogre::String gameObjectParent = getAttrib(XMLNode, "parent", "invalid");
	Ogre::StringUtil::toLowerCase(gameObjectName);
	Ogre::String gameObjectTag = getAttrib(XMLNode, "tag", "invalid");
	Ogre::StringUtil::toLowerCase(gameObjectName);
	bool active = getAttribBool(XMLNode, "active", false);

	if (gameObjectName == "invalid"|| gameObjectParent == "invalid" || gameObjectTag == "invalid" )
		messageError = messageError + " [ArchetypeLoader] Error: Incorrect loading of the game object attributes: " + 
		gameObjectName +"_"+ gameObjectParent +"_"+ gameObjectTag + "\n";
	else {
		// create a gameobjects with its name and tag 
		GameObject* obj = new GameObject(gameObjectName, gameObjectParent, gameObjectTag, active);

		std::list<Component*> components;

		processComponents(XMLNode, components, messageError);

		EntityComponentManager::Instance()->registerPrototype(gameObjectName, new Prototype(obj, components));
	}
}

void ArchetypeLoader::processComponents(rapidxml::xml_node<>* XMLNode, std::list<Component*>& components, Ogre::String &messageError) {
	rapidxml::xml_node<>* pElement;

	// Process node (*)
	pElement = XMLNode->first_node("component");
	while (pElement)
	{
		processComponent(pElement, components, messageError);
		pElement = pElement->next_sibling("component");
	}
}

void ArchetypeLoader::processComponent(rapidxml::xml_node<>* XMLNode, std::list<Component*>& components, Ogre::String &messageError) {
	Ogre::String componentName = getAttrib(XMLNode, "name", "");
	if (componentName == "")
		messageError = messageError + " [ArchetypeLoader] Error: Incorrect loading of the component name \n";
	else {

		Component* component = EntityComponentManager::Instance()->getFactory(componentName)->create();

		rapidxml::xml_node<>* pElement;
		pElement = XMLNode->first_node("user_data");
		params.clear();

		while (pElement)
		{
			processDataParametres(pElement, messageError);
			pElement = pElement->next_sibling("user_data");
		}
		component->load(params);
		components.push_back(component);
	}
}

