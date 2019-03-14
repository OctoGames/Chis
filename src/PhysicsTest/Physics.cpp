#include "Physics.h"

Physics* Physics::instance_ = nullptr;

Physics::Physics() : numberOfRigidBodies_(0)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

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

	//dynamicsWorld->getDebugDrawer()->drawLine(from, to, btVector4(1, 0, 0, 1));

	return true;
}

btRigidBody* Physics::getRigidBodyByName(std::string name)
{
	return physicsAccessors.find(name)->second;
}

void Physics::debugMode()
{
	debugDrawer* dbg_drawer = &debugDrawer::getSingleton();	// DebugDrawer derives btIDebugDraw, see below for my definition
	dbg_drawer->init();
	dbg_drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints + btIDebugDraw::DBG_DrawConstraints + btIDebugDraw::DBG_DrawConstraintLimits);
	//dbg_drawer->setDebugMode(1);
	dbg_drawer->debugPhysics(true);
	dynamicsWorld->setDebugDrawer(dbg_drawer);

	btVector3 from(0, 20, 0);
	btVector3 to(30, 20, 0);
	
	dynamicsWorld->getDebugDrawer()->drawSphere(from, 10, btVector3(1,0,0));
}

void Physics::createRigidBody(Ogre::SceneNode * node, double mass, std::string name)
{
	btTransform Transform;
	btVector3 v;
	v.setX(node->getPosition().x);
	v.setY(node->getPosition().y);
	v.setZ(node->getPosition().z);

	Transform.setIdentity();
	Transform.setOrigin(v);
	Transform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));

	btScalar Mass(mass); 
	btVector3 localInertia(0, 0, 0);

	btCollisionShape* Shape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	Shape->calculateLocalInertia(Mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo RBInfo(Mass, MotionState, Shape, localInertia);

	btRigidBody *Body = new btRigidBody(RBInfo);

	Body->setUserPointer(node);

	//add the body to the dynamics world

	addToPhysicWorld(Body);

	trackRigidBodyWithName(Body, name);

	debugMode();
}