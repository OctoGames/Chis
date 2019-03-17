#ifndef SCENELOADER_H_
#define SCENELOADER_H_
#include "DotSceneLoader.h"
#include "LoadArchetypes.h"

// Read the scene and create the objects from the list of components of each one
class SceneLoader : public DotSceneLoader {
public:
	SceneLoader();
	virtual ~SceneLoader();
	
	virtual void processScene(rapidxml::xml_node<>* XMLRoot);
	virtual void processNodes(rapidxml::xml_node<>* XMLNode);
	virtual void processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
	virtual void processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);


};
#endif SCENELOADER_H_