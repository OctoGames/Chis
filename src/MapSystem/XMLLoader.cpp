#include "XMLLoader.h"



XMLLoader::XMLLoader()
{
}


XMLLoader::~XMLLoader()
{
}


Ogre::String XMLLoader::getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue)
{
	if (XMLNode->first_attribute(attrib.c_str()))
		return XMLNode->first_attribute(attrib.c_str())->value();
	else
		return defaultValue;
}

Ogre::Real XMLLoader::getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue)
{
	if (XMLNode->first_attribute(attrib.c_str()))
		return Ogre::StringConverter::parseReal(XMLNode->first_attribute(attrib.c_str())->value());
	else
		return defaultValue;
}

bool XMLLoader::getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue)
{
	if (!XMLNode->first_attribute(attrib.c_str()))
		return defaultValue;

	if (Ogre::String(XMLNode->first_attribute(attrib.c_str())->value()) == "1")
		return true;

	return false;
}

Ogre::Vector3 XMLLoader::parseVector3(rapidxml::xml_node<>* XMLNode)
{
	return Ogre::Vector3(
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value())
	);
}

Ogre::Quaternion XMLLoader::parseQuaternion(rapidxml::xml_node<>* XMLNode)
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

Ogre::ColourValue XMLLoader::parseColour(rapidxml::xml_node<>* XMLNode)
{
	return Ogre::ColourValue(
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("r")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("g")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("b")->value()),
		XMLNode->first_attribute("a") != NULL ? Ogre::StringConverter::parseReal(XMLNode->first_attribute("a")->value()) : 1
	);
}

Ogre::String XMLLoader::getGameObjectTag(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError) {
	Ogre::String tag = "";
	rapidxml::xml_node<>* pElement;
	pElement = XMLNode->first_node("user_data");
	bool findTag = false;

	while (pElement && !findTag)
	{
		Ogre::String nameProp = getAttrib(pElement, "name");
		Ogre::StringUtil::toLowerCase(nameProp);
		Ogre::String typeProp = getAttrib(pElement, "type");
		Ogre::StringUtil::toLowerCase(typeProp);

		if (nameProp == "tag" && typeProp == "str") {
			tag = getAttrib(pElement, "value", "");
			if (tag == "")
				messageError = messageError + " [SceneLoader] Error:  Incorrect loading of the game object tag \n";
			findTag = true;
		}

		pElement = pElement->next_sibling("user_data");
	}
	return tag;
}

void XMLLoader::processDataParametres(rapidxml::xml_node<>* XMLNode, Ogre::String &messageError) {

	Ogre::String nameProp = getAttrib(XMLNode, "name", "");
	Ogre::StringUtil::toLowerCase(nameProp);
	Ogre::String typeProp = getAttrib(XMLNode, "type", "");
	Ogre::StringUtil::toLowerCase(typeProp);

	if (nameProp == "" && typeProp == "")
		messageError = messageError + " [SceneLoader] Error: Incorrect loading of the user_data attributes \n";
	else {
		if (typeProp == "str")
			params[nameProp].s = getAttrib(XMLNode, "value");
		else if (typeProp == "int")
			params[nameProp].i = getAttribBool(XMLNode, "value");
		else if (typeProp == "float")
			params[nameProp].f = getAttribReal(XMLNode, "value");
	}
}