#ifndef LOADARCHETYPE_H_
#define LOADARCHETYPE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
// This class is used to load the names of the components of the game objects of the archetype files
class LoadArchetypes
{
public:
	static LoadArchetypes* Instance();

	std::list<std::string*> getComponentsList(const std::string& gameObject);

private:
	LoadArchetypes();

	std::list<std::string*> loadArchetype(const std::string& gameObject);

	static LoadArchetypes* instance_;

	// stores the names of the game objects and its respective components
	std::map<std::string, std::list<std::string*>> gameObjects_;

};

#endif LOADARCHETYPE_H_