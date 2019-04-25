#ifndef LOADARCHETYPE_H_
#define LOADARCHETYPE_H_

#include "XMLLoader.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;
// This class is used to load the names of the components of the game objects of the archetype files
class ArchetypeLoader : public XMLLoader
{
public:
	static ArchetypeLoader* Instance();

	void loadArchetypes();

private:

	static ArchetypeLoader* instance_;

	const char* PATH_ = "Assets/Archetypes/";

	ArchetypeLoader();
	
	void readArchetypeFile(const std::string& file, Ogre::String &messageError);
	void processArchetypes(rapidxml::xml_node<>* XMLRoot, Ogre::String &messageError);
	void processEntity(rapidxml::xml_node<>* XMLRoot, Ogre::String &messageError);
	void processComponents(rapidxml::xml_node<>* XMLNode, std::list<Component*>& components, Ogre::String &messageError);
	void processComponent(rapidxml::xml_node<>* XMLNode, std::list<Component*>& components, Ogre::String &messageError);

};

#endif LOADARCHETYPE_H_