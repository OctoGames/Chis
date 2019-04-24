#ifndef SCENELOADER_H_
#define SCENELOADER_H_

#include "LoadArchetypes.h"
#include "gameobject.h"
#include "MeshRenderer.h"
#include "RigidBody.h"
#include "rapidxml.hpp"

#include <vector>
#include <Ogre.h>

// Read the scene and create the objects from the list of components of each one
class SceneLoader {


private:
	SceneLoader();

	void processScene(rapidxml::xml_node<>* XMLRoot);
	void processNodes(rapidxml::xml_node<>* XMLNode);
	void processNode(rapidxml::xml_node<>* XMLNode);
	//void processEntity(rapidxml::xml_node<>* XMLNode);

	void loadComponent(const std::string& c, rapidxml::xml_node<>* XMLNode, GameObject* gameObject);
	void loadAudioSource(rapidxml::xml_node<>* XMLNode, GameObject* gameObject);
	void loadLight(rapidxml::xml_node<>* XMLNode, GameObject* gameObject);
	void loadMeshRenderer(rapidxml::xml_node<>* XMLNode, GameObject* gameObject);
	void loadRigidBody(rapidxml::xml_node<>* XMLNode, GameObject* gameObject);
	void loadTransform(rapidxml::xml_node<>* XMLNode, GameObject* gameObject);
	void loadVertexTransform(rapidxml::xml_node<>* XMLNode, int vertexID);



	Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue = "");
	Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue = 0);
	bool getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue = false);
	Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode);
	Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode);
	Ogre::ColourValue parseColour(rapidxml::xml_node<>* XMLNode);

	std::vector<Ogre::Vector3> vertexPositions;
	static SceneLoader* instance_;

	const char* PATH_ = "Assets/Scenes/";
public:
	static SceneLoader* Instance();

	void loadScene(const Ogre::String& SceneName);
	inline std::vector <Ogre::Vector3> getVertex() {
		return vertexPositions;
	}
};
#endif SCENELOADER_H_