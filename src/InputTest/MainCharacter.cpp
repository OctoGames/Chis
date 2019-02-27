#include "MainCharacter.h"



MainCharacter::MainCharacter(Ogre::SceneManager* sManager)
{
	mainCharacterNode = sManager->getRootSceneNode()->createChildSceneNode("mainCharacter");

	//ent = sManager->createEntity("fish.mesh");

	//mainCharacterNode->attachObject(ent);

	mainCharacterNode->setPosition(0, 70, 1000);
	//mainCharacterNode->setScale(30, 30, 30);
	//mainCharacterNode->yaw(Ogre::Degree(-90));
	mainCharacterNode->setVisible(false);

}


MainCharacter::~MainCharacter()
{
}
