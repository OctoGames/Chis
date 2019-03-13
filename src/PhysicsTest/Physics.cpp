#include "Physics.h"

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


bool Physics::update()
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
			}
		}
	}

	return true;
}

btRigidBody* Physics::getRigidBodyByName(std::string name)
{
	return physicsAccessors.find(name)->second;
}

void Physics::createRigidBody(btVector3 position, double mass, std::string meshName, Ogre::SceneNode* node)
{
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(position);
	Transform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));

	btScalar Mass(mass); 
	btVector3 localInertia(0, 0, 0);

	Ogre::MeshPtr MeshPtr = Ogre::Singleton<Ogre::MeshManager>::getSingletonPtr()->getByName(meshName);
	MeshStrider* Strider = new MeshStrider(MeshPtr.get());

	btCollisionShape* Shape = new btBvhTriangleMeshShape(Strider, true, true);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	Shape->setUserPointer(node);

	Shape->setUserIndex(1);

	Shape->calculateLocalInertia(Mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo RBInfo(Mass, MotionState, Shape, localInertia);

	btRigidBody *Body = new btRigidBody(RBInfo);

	Body->setUserPointer(node);

	//add the body to the dynamics world

	addToPhysicWorld(Body);
	numberOfRigidBodies_++;

	dynamicsWorld->addRigidBody(Body);

	trackRigidBodyWithName(Body, meshName);
}