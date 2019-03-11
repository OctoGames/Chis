#include "Physics.h"
#include <iostream>


Physics::Physics() : numberOfRigidBodies_(0)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

}

Physics::~Physics()
{

}


bool Physics::frameStarted()
{

	dynamicsWorld->stepSimulation(1.0f / 60.0f); //suppose you have 60 frames per second

	for (int i = 0; i < numberOfRigidBodies_; i++)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);

			void *userPointer = body->getUserPointer();
			if (userPointer)
			{
				btQuaternion orientation = trans.getRotation();
				Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
				sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
				sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));

				std::cout << sceneNode->getPosition() << std::endl;
			}
		}
	}

	return true;
}

btRigidBody* Physics::getRigidBodyByName(std::string name)
{
	return physicsAccessors.find(name)->second;	
}