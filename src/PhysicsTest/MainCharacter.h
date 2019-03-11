#pragma once
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

//Garbage collector
//#include "CheckML.h"

class MainCharacter
{

private:
	Ogre::Entity* ent;
	Ogre::SceneNode* mainCharacterNode;

public:
	MainCharacter(Ogre::SceneManager* sManager);
	~MainCharacter();

	Ogre::SceneNode* getMainCharacterNode() { return mainCharacterNode; };
};

