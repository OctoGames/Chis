#ifndef __ARCHETYPE_LOADER_H__
#define __ARCHETYPE_LOADER_H__

#include <filesystem>

#include "XMLLoader.h"

namespace fs = std::experimental::filesystem;

// This class is used to load the names of the components of the game objects of the archetype files
class ArchetypeLoader : public XMLLoader
{
public:
	static ArchetypeLoader* Instance();
	void loadArchetypes();

private:
	ArchetypeLoader();

	void readArchetypeFile(const std::string& file, Ogre::String &messageError);
	void processArchetypes(rapidxml::xml_node<>* XMLRoot, Ogre::String &messageError);
	void processEntity(rapidxml::xml_node<>* XMLRoot, Ogre::String &messageError);
	void processComponents(rapidxml::xml_node<>* XMLNode, std::list<Component*>& components, Ogre::String &messageError);
	void processComponent(rapidxml::xml_node<>* XMLNode, std::list<Component*>& components, Ogre::String &messageError);

	static ArchetypeLoader* instance_;

	const char* PATH_ = "Assets/Archetypes/";
};

#endif // !__ARCHETYPE_LOADER_H__