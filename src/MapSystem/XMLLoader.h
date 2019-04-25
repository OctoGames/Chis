#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <Ogre.h>
#include "rapidxml.hpp"
#include "EntityComponentManager.h"

class XMLLoader
{
public:
	XMLLoader();
	~XMLLoader();
protected:
	Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue = "");
	Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue = 0);
	bool getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue = false);
	Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode);
	Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode);
	Ogre::ColourValue parseColour(rapidxml::xml_node<>* XMLNode);
	Ogre::String getGameObjectTag(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError);
	void processDataParametres(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError);

	std::map<std::string, ValueType> params;
};

