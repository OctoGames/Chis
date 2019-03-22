#include "Physics.h"
#include <iostream>

Physics* Physics::instance_ = nullptr;

Physics::Physics() : numberOfRigidBodies_(0),
debug_(false),
visibleDebug_(false)
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

	castRays();

	if (debug_)
	{
		debugDrawer::Instance()->resetLineNumber();

		for (int i = 0; i < debugObjects.size(); i++)
		{
			Ogre::SceneNode* n = debugObjects[i].node;

			if (!visibleDebug_)
				n->setVisible(false);
			else
				n->setVisible(true);

			btVector3 v;
			v.setX(n->getPosition().x);
			v.setY(n->getPosition().y);
			v.setZ(n->getPosition().z);

			btQuaternion quaternion;

			quaternion.setX(n->getOrientation().x);
			quaternion.setY(n->getOrientation().y);
			quaternion.setZ(n->getOrientation().z);
			quaternion.setW(n->getOrientation().w);

			if(debugObjects[i].type == "box")
				debugDrawer::Instance()->drawCube(v, debugObjects[i].scale, quaternion);
			else if (debugObjects[i].type == "sphere")
			{
				debugDrawer::Instance()->drawSphere(v, debugObjects[i].radious);
			}
		}
	}

	return true;
}

void Physics::toggleDebug()
{
	debug_ = !debug_; 

	if (!debug_) {
		debugDrawer::Instance()->resetLineNumber();

		for (int i = 0; i < debugObjects.size(); i++)
		{
			Ogre::SceneNode* n = debugObjects[i].node;

			n->setVisible(true);
		}
	}
}

void Physics::toggleDebugMode()
{

	if(debug_)
		 visibleDebug_ = !visibleDebug_;
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

void Physics::createBoxRigidBody(Ogre::SceneNode * node, double mass, Ogre::Vector3 scale, std::string name)
{

	btVector3 v3;
	v3.setX(scale.x);
	v3.setY(scale.y);
	v3.setZ(scale.z);

	btTransform transform = setTransform(node);

	btCollisionShape* shape = new btBoxShape(v3);

	setRigidBody(shape, mass, name, node, transform);

	setDebugObject(node, "box", 0, v3);

}

void Physics::createSphereRididBody(Ogre::SceneNode * node, double mass, double radious, std::string name)
{
	
	btTransform transform = setTransform(node);

	btCollisionShape* shape = new btSphereShape(radious);

	setRigidBody(shape, mass, name, node, transform);

	setDebugObject(node, "sphere", radious, btVector3(radious, radious, radious));
}

void Physics::setDebugObject(Ogre::SceneNode * node, std::string type, double radious, btVector3 scale)
{
	debugObjectsPropierties dbp;
	dbp.node = node; dbp.radious = radious; dbp.scale = scale; dbp.type = type;

	debugObjects.push_back(dbp);
}

void Physics::setRigidBody(btCollisionShape* shape, double mass, std::string name, Ogre::SceneNode* node, btTransform transform)
{
	btScalar Mass(mass);
	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState *MotionState = new btDefaultMotionState(transform);

	shape->calculateLocalInertia(Mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo RBInfo(Mass, MotionState, shape, localInertia);

	btRigidBody *Body = new btRigidBody(RBInfo);

	addToPhysicWorld(Body);

	trackRigidBodyWithName(Body, name);

	Body->setUserPointer(node);
}

btTransform Physics::setTransform(Ogre::SceneNode* node)
{
	btTransform transform;
	btVector3 v;
	v.setX(node->getPosition().x);
	v.setY(node->getPosition().y);
	v.setZ(node->getPosition().z);

	transform.setIdentity();
	transform.setOrigin(v);
	//Transform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));

	return transform;
}

void Physics::castRays()
{
	static float up = 0.f;
	static float dir = 1.f;
	//add some simple animation
	//if (!m_idle)
	{
		up += 0.01 * dir;

		if (btFabs(up) > 2)
		{
			dir *= -1.f;
		}

		btTransform tr = dynamicsWorld->getCollisionObjectArray()[1]->getWorldTransform();
		static float angle = 0.f;
		angle += 0.01f;
		tr.setRotation(btQuaternion(btVector3(0, 1, 0), angle));
		dynamicsWorld->getCollisionObjectArray()[1]->setWorldTransform(tr);
	}

	///step the simulation
	if (dynamicsWorld)
	{
		dynamicsWorld->updateAabbs();
		dynamicsWorld->computeOverlappingPairs();

		btVector3 red(1, 0, 0);
		btVector3 blue(0, 0, 1);

		///all hits
		{
			btVector3 from(-30, 1 + up, 0);
			btVector3 to(30, 1, 0);
			dynamicsWorld->getDebugDrawer()->drawLine(from, to, btVector4(0, 0, 0, 1));
			btCollisionWorld::AllHitsRayResultCallback allResults(from, to);
			allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
			//kF_UseGjkConvexRaytest flag is now enabled by default, use the faster but more approximate algorithm
			//allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
			allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;

			dynamicsWorld->rayTest(from, to, allResults);

			for (int i = 0; i < allResults.m_hitFractions.size(); i++)
			{
				btVector3 p = from.lerp(to, allResults.m_hitFractions[i]);
				dynamicsWorld->getDebugDrawer()->drawSphere(p, 0.1, red);
				dynamicsWorld->getDebugDrawer()->drawLine(p, p + allResults.m_hitNormalWorld[i], red);
			}
		}

		///first hit
		{
			btVector3 from(-30, 1.2, 0);
			btVector3 to(30, 1.2, 0);
			dynamicsWorld->getDebugDrawer()->drawLine(from, to, btVector4(0, 0, 1, 1));

			btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
			closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

			dynamicsWorld->rayTest(from, to, closestResults);

			if (closestResults.hasHit())
			{
				btVector3 p = from.lerp(to, closestResults.m_closestHitFraction);
				dynamicsWorld->getDebugDrawer()->drawSphere(p, 0.1, blue);
				dynamicsWorld->getDebugDrawer()->drawLine(p, p + closestResults.m_hitNormalWorld, blue);
			}
		}
	}
}