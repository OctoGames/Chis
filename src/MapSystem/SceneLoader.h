#ifndef SCENELOADER_H_
#define SCENELOADER_H_

#include "XMLLoader.h"

// Read the scene and create the objects from the list of components of each one
class SceneLoader : public XMLLoader {


private:
	SceneLoader();

	void processScene(rapidxml::xml_node<>* XMLRoot, Ogre::String &messageError);
	void processGameObjects(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError);
	void processGameObject(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError);
	void processComponentsData(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError);

	static SceneLoader* instance_;

	const char* PATH_ = "Assets/Scenes/";

public:
	static SceneLoader* Instance();

	void loadScene(const Ogre::String& SceneName);
};
#endif SCENELOADER_H_