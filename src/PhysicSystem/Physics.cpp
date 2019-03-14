#include "Physics.h"

Physics* Physics::instance_ = nullptr;

Physics::Physics() : numberOfRigidBodies_(0),
debug_(false)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	debugMode();

}

Physics* Physics::Instance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Physics();
	}
	return instance_;
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

	if (debug_)
	{
		debugDrawer::Instance()->resetLineNumber();

		for (int i = 0; i < debugObjects.size(); i++)
		{
			Ogre::SceneNode* n = debugObjects[i].node;

			btVector3 v;
			v.setX(n->getPosition().x);
			v.setY(n->getPosition().y);
			v.setZ(n->getPosition().z);

			debugDrawer::Instance()->drawCube(v, debugObjects[i].scale);
		}
	}

	return true;
}

btRigidBody* Physics::getRigidBodyByName(std::string name)
{
	return physicsAccessors.find(name)->second;
}

void Physics::debugMode()
{
	dbg_drawer = &debugDrawer::getSingleton();	// DebugDrawer derives btIDebugDraw, see below for my definition

	dynamicsWorld->setDebugDrawer(dbg_drawer);
}

void Physics::createRigidBody(Ogre::SceneNode * node, double mass, btVector3 scale, std::string name)
{
	btTransform transform;
	btVector3 v;
	v.setX(node->getPosition().x);
	v.setY(node->getPosition().y);
	v.setZ(node->getPosition().z);

	transform.setIdentity();
	transform.setOrigin(v);
	//Transform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));

	btScalar Mass(mass); 
	btVector3 localInertia(0, 0, 0);

	btCollisionShape* Shape = new btBoxShape(scale);

	btDefaultMotionState *MotionState = new btDefaultMotionState(transform);

	Shape->calculateLocalInertia(Mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo RBInfo(Mass, MotionState, Shape, localInertia);

	btRigidBody *Body = new btRigidBody(RBInfo);

	//add the body to the dynamics world

	addToPhysicWorld(Body);

	trackRigidBodyWithName(Body, name);

	debugObjectsPropierties dbp;
	dbp.node = node; dbp.scale = scale;

	debugObjects.push_back(dbp);

	debugDrawer::Instance()->drawCube(v, scale);

	Body->setUserPointer(node);
}