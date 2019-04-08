#include "LoadArchetypes.h"

LoadArchetypes* LoadArchetypes::instance_ = nullptr;

LoadArchetypes::LoadArchetypes()
{
}

LoadArchetypes* LoadArchetypes::Instance()
{
	if (instance_ == nullptr) {
		instance_ = new LoadArchetypes();
	}
	return instance_;
}

std::list<std::string> LoadArchetypes::loadArchetype(const std::string& gameObject)
{
	std::fstream file;
	std::string name = PATH_ + gameObject + ".components";
	std::string component;
	std::list<std::string> componentList;

	file.open(name.c_str(), std::ios::in);
	if (file.is_open()) {
		getline(file, component);
		while (!file.eof()) {
			Ogre::StringUtil::toLowerCase(component);
			componentList.push_back(component);
			getline(file, component);
		}
		file.close();
	}else
		Ogre::LogManager::getSingleton().logMessage("++++++++++++++++++++++Error al cargar el Arquetipo++++++++++++++++++++++ \n Ruta del archivo = " + name);

	gameObjects_.insert(std::pair< std::string, std::list<std::string>>
		(gameObject, componentList));
	
	return componentList;
}

std::list<std::string> LoadArchetypes::getComponentsList(const std::string& gameObject)
{
	std::map<std::string, std::list<std::string>>
		::iterator it = gameObjects_.find(gameObject);

	if (it == gameObjects_.end()) {
		return loadArchetype(gameObject);
	}

	return (*it).second;
}
