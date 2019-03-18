#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H

// Includes
#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreColourValue.h>
#include <OgreResourceGroupManager.h>
#include <vector>

#include <OgreSceneLoader.h>

#include <Ogre.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainMaterialGeneratorA.h>

#include <OgreSceneLoaderManager.h>

#include "rapidxml.hpp"

	// Forward declarations
namespace Ogre
{
	class SceneManager;
	class SceneNode;
	class TerrainGroup;
	class TerrainGlobalOptions;
}

class DotSceneLoader : public Ogre::SceneLoader
{
public:
	Ogre::TerrainGlobalOptions *mTerrainGlobalOptions;

	DotSceneLoader();
	virtual ~DotSceneLoader();

	void load(Ogre::DataStreamPtr& stream, const Ogre::String& groupName,
		Ogre::SceneNode* rootNode);

	void parseDotScene(const Ogre::String& SceneName,
		const Ogre::String& groupName,
		Ogre::SceneNode* pAttachNode,
		const Ogre::String& sPrependNode = "");

	Ogre::TerrainGroup* getTerrainGroup() { return mTerrainGroup; }

	const Ogre::ColourValue& getBackgroundColour() {
		return mBackgroundColour;
	}

	std::vector<Ogre::String> staticObjects;
	std::vector<Ogre::String> dynamicObjects;
protected:
	virtual void processScene(rapidxml::xml_node<>* XMLRoot);

	virtual void processNodes(rapidxml::xml_node<>* XMLNode);
	void processExternals(rapidxml::xml_node<>* XMLNode);
	void processEnvironment(rapidxml::xml_node<>* XMLNode);
	void processTerrain(rapidxml::xml_node<>* XMLNode);
	void processTerrainPage(rapidxml::xml_node<>* XMLNode);
	void processBlendmaps(rapidxml::xml_node<>* XMLNode);
	void processUserData(rapidxml::xml_node<>* XMLNode, Ogre::UserObjectBindings& userData);
	void processLight(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
	void processCamera(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);

	virtual void processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
	void processLookTarget(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
	void processTrackTarget(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
	virtual void processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
	void processParticleSystem(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
	void processBillboardSet(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
	void processPlane(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);

	void processFog(rapidxml::xml_node<>* XMLNode);
	void processSkyBox(rapidxml::xml_node<>* XMLNode);
	void processSkyDome(rapidxml::xml_node<>* XMLNode);
	void processSkyPlane(rapidxml::xml_node<>* XMLNode);
	void processClipping(rapidxml::xml_node<>* XMLNode);

	void processLightRange(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);
	void processLightAttenuation(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);

	Ogre::SceneManager *mSceneMgr;
	Ogre::SceneNode *mAttachNode;
	Ogre::String m_sGroupName;
	Ogre::String m_sPrependNode;
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::Vector3 mTerrainPosition;
	Ogre::ColourValue mBackgroundColour;


	// Parse Attributes
	Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue = "")
	{
		if (XMLNode->first_attribute(attrib.c_str()))
			return XMLNode->first_attribute(attrib.c_str())->value();
		else
			return defaultValue;
	}

	Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue = 0)
	{
		if (XMLNode->first_attribute(attrib.c_str()))
			return Ogre::StringConverter::parseReal(XMLNode->first_attribute(attrib.c_str())->value());
		else
			return defaultValue;
	}

	bool getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue = false)
	{
		if (!XMLNode->first_attribute(attrib.c_str()))
			return defaultValue;

		if (Ogre::String(XMLNode->first_attribute(attrib.c_str())->value()) == "true" ||
			Ogre::String(XMLNode->first_attribute(attrib.c_str())->value()) == "True")
			return true;

		return false;
	}

	Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode)
	{
		return Ogre::Vector3(
			Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value()),
			Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value()),
			Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value())
		);
	}

	Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode)
	{
		//! @todo Fix this crap!

		Ogre::Quaternion orientation;

		if (XMLNode->first_attribute("qw"))
		{
			orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
			orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
			orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
			orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
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

	Ogre::ColourValue parseColour(rapidxml::xml_node<>* XMLNode)
	{
		return Ogre::ColourValue(
			Ogre::StringConverter::parseReal(XMLNode->first_attribute("r")->value()),
			Ogre::StringConverter::parseReal(XMLNode->first_attribute("g")->value()),
			Ogre::StringConverter::parseReal(XMLNode->first_attribute("b")->value()),
			XMLNode->first_attribute("a") != NULL ? Ogre::StringConverter::parseReal(XMLNode->first_attribute("a")->value()) : 1
		);
	}

};

#endif // DOT_SCENELOADER_H
