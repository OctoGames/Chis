#include "SceneLoader.h"
//#include "gameobject.h"
//#include <Transform.h>

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
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error: Invalid .scene File. Missing <scene>");
		delete scene;
		return;
	}

	// Process the scene
	processScene(XMLRoot);

	delete scene;
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

void SceneLoader::processNode(rapidxml::xml_node<>* XMLNode) {

	rapidxml::xml_node<>* pElement;
	// Get the gameobject name
	Ogre::String gameObjectName = getAttrib(XMLNode, "name");
	Ogre::String gameObjectTag = "";
	Ogre::StringUtil::toLowerCase(gameObjectName);

	//-----look for the gameobject tag-----//
	pElement = XMLNode->first_node("user_data");
	bool findTag = false;
	bool vertex = false;
	int vertexID;



	while (pElement && !findTag)
	{
		Ogre::String nameProp = getAttrib(pElement, "name");
		Ogre::String typeProp = getAttrib(pElement, "type");

		Ogre::StringUtil::toLowerCase(nameProp);
		if (nameProp == "tag" && typeProp == "str") {
			gameObjectTag = getAttrib(pElement, "value");
			findTag = true;
		}
		else if (nameProp == "vertex") {
			vertexID = getAttribReal(pElement, "value");
			findTag = true;
			vertex = true;
		}

		pElement = pElement->next_sibling("user_data");
	}
	if (vertex) {
		if (vertexID >= vertexPositions.size()) {
			vertexPositions.resize(vertexID + 1);
			Ogre::LogManager::getSingleton().logMessage("++++++++++++++++++++++ Tamaño vector" + std::to_string(vertexPositions.size()));

		}
		loadVertexTransform(XMLNode, vertexID);
		Ogre::LogManager::getSingleton().logMessage("++++++++++++++++++++++ Número de vértice " + std::to_string(vertexID));
		vertex = false;
	}
	else {
		//-----look for the gameobject tag-----//
			// create a gameobjects with its name and tag 
		GameObject* g = new GameObject(gameObjectName, gameObjectTag);

		// lista de componentes del gameobject
		std::list<std::string> componentsList = LoadArchetypes::Instance()->getComponentsList(gameObjectTag);
		if (componentsList.empty())
			Ogre::LogManager::getSingleton().logMessage("++++++++++++++++++++++ Error: lista de componentes vacía +++++++++++++++++++++++++++");
		// create components from the list of components with its parametres
		for (std::string c : componentsList)
		{
			loadComponent(c, XMLNode, g);
		}
	}
}


void SceneLoader::loadComponent(const std::string& c, rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
{
	Ogre::String componentString = c;
	Ogre::StringUtil::toLowerCase(componentString);

	if (componentString == "audiosource") {
		loadAudioSource(XMLNode, gameObject);
	}
	else if (componentString == "camera") {
		//???
	}
	else if (componentString == "light") {
		loadLight(XMLNode, gameObject);
	}
	else if (componentString == "meshrenderer") {
		loadMeshRenderer(XMLNode, gameObject);
	}

	else if (componentString == "rigidbody") {
		loadRigidBody(XMLNode, gameObject);
	}

	else if (componentString == "transform") {
		loadTransform(XMLNode, gameObject);
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
	if (meshFile == "")
		Ogre::LogManager::getSingleton().logMessage("++++++++++++++++++++++ Error al leer el meshFile +++++++++++++++++++++++++++");
	MeshRenderer* renderer = new MeshRenderer(gameObject, meshFile);

	//no hace falta hacer el set material, lo coge automáticamente(ni idea de como pero lo coge y es genial)
}

void SceneLoader::loadRigidBody(rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
{
	rapidxml::xml_node<>* pElement;
	RigidBody* rigidBody = new RigidBody(gameObject);

	// Find mass and rigid body type attributes
	pElement = XMLNode->first_node("user_data");
	Ogre::String radioSphereRB, massValue;
	double mass = 0.0;
	bool findRigidBodySphere = false, findMass = false;

	while (pElement && (!findRigidBodySphere || !findMass))
	{
		Ogre::String nameProp = getAttrib(pElement, "name");
		Ogre::String typeProp = getAttrib(pElement, "type");
		Ogre::StringUtil::toLowerCase(nameProp);
		Ogre::StringUtil::toLowerCase(typeProp);

		if (!findRigidBodySphere && nameProp == "sphererb" && typeProp == "float") {
			radioSphereRB = getAttrib(pElement, "value");
			findRigidBodySphere = true;
		}

		else if (!findMass && nameProp == "mass" && typeProp == "float") {
			massValue = getAttrib(pElement, "value");
			mass = (double)Ogre::StringConverter::parseReal(massValue);
			findMass = true;
		}
		pElement = pElement->next_sibling("user_data");
	}

	if (findRigidBodySphere) {
		rigidBody->createSphereRB(mass, (double)Ogre::StringConverter::parseReal(radioSphereRB), gameObject->getName() + "RB");
	}
	else {
		pElement = XMLNode->first_node("scale");

		Ogre::Vector3 v3 = parseVector3(pElement);
		v3.y = v3.y*0.05;

		rigidBody->createBoxRB(mass, v3, gameObject->getName() + "RB");
	}
}

void SceneLoader::loadTransform(rapidxml::xml_node<>* XMLNode, GameObject* gameObject)
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

void SceneLoader::loadVertexTransform(rapidxml::xml_node<>* XMLNode, int vertexID) {
	rapidxml::xml_node<>* pElement;

	// Process position (?)
	pElement = XMLNode->first_node("position");
	if (pElement) vertexPositions[vertexID] = parseVector3(pElement);
}


Ogre::String SceneLoader::getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue)
{
	if (XMLNode->first_attribute(attrib.c_str()))
		return XMLNode->first_attribute(attrib.c_str())->value();
	else
		return defaultValue;
}

Ogre::Real SceneLoader::getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue)
{
	if (XMLNode->first_attribute(attrib.c_str()))
		return Ogre::StringConverter::parseReal(XMLNode->first_attribute(attrib.c_str())->value());
	else
		return defaultValue;
}

bool SceneLoader::getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue)
{
	if (!XMLNode->first_attribute(attrib.c_str()))
		return defaultValue;

	if (Ogre::String(XMLNode->first_attribute(attrib.c_str())->value()) == "true")
		return true;

	return false;
}

Ogre::Vector3 SceneLoader::parseVector3(rapidxml::xml_node<>* XMLNode)
{
	return Ogre::Vector3(
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value())
	);
}

Ogre::Quaternion SceneLoader::parseQuaternion(rapidxml::xml_node<>* XMLNode)
{
	//! @todo Fix this crap!

	Ogre::Quaternion orientation;

	if (XMLNode->first_attribute("qw"))
	{
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
	}
	else if (XMLNode->first_attribute("axisX"))
	{
		Ogre::Vector3 axis;
		axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisX")->value());
		axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisY")->value());
		axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisZ")->value());
		Ogre::Real angle = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angle")->value());;
		orientation.FromAngleAxis(Ogre::Angle(angle), axis);
	}
	else if (XMLNode->first_attribute("angleX"))
	{
		Ogre::Matrix3 rot;
		rot.FromEulerAnglesXYZ(
			Ogre::StringConverter::parseAngle(XMLNode->first_attribute("angleX")->value()),
			Ogre::StringConverter::parseAngle(XMLNode->first_attribute("angleY")->value()),
			Ogre::StringConverter::parseAngle(XMLNode->first_attribute("angleZ")->value()));
		orientation.FromRotationMatrix(rot);
	}
	else if (XMLNode->first_attribute("x"))
	{
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
	}
	else if (XMLNode->first_attribute("w"))
	{
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
	}

	return orientation;
}

Ogre::ColourValue SceneLoader::parseColour(rapidxml::xml_node<>* XMLNode)
{
	return Ogre::ColourValue(
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("r")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("g")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("b")->value()),
		XMLNode->first_attribute("a") != NULL ? Ogre::StringConverter::parseReal(XMLNode->first_attribute("a")->value()) : 1
	);
}
