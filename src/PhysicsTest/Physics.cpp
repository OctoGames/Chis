#include "Physics.h"

Physics::Physics()
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


bool Physics::frameStarted(const Ogre::FrameEvent &evt)
{
	if (this->physicsEngine != NULL)
	{
		physicsEngine->getDynamicsWorld()->stepSimulation(1.0f / 60.0f); //suppose you have 60 frames per second

		for (int i = 0; i < this->physicsEngine->getCollisionObjectCount(); i++)
		{
			btCollisionObject* obj = this->physicsEngine->getDynamicsWorld()->getCollisionObjectArray()[i];
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
				}
			}
		}
	}
	return true;
}
