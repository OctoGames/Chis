#include "Physics.h"
#include <iostream>

Physics* Physics::instance_ = nullptr;

Physics::Physics() : numberOfRigidBodies_(0),
debug_(false),
visibleDebug_(true)
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

	debugDrawer::Instance()->resetLineNumber();

	castRays();

	if (debug_)
	{
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

			if(debugObjects[i].type == "box")
				debugDrawer::Instance()->drawCube(v, debugObjects[i].scale);
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

void Physics::getRaycastValuesByName(const std::string name, btVector3& from, btVector3& to)
{
	bool found = false;
	int i = 0;

	while (i < rayCasts_.size() && !found)
	{
		if (name == rayCasts_[i].rayName)
		{
			from = rayCasts_[i].from;
			to = rayCasts_[i].to;
			found = true;
		}
		else i++;
	}
}

Ogre::SceneNode* Physics::getRaycastCollisionByName(const std::string name)
{
	bool found = false;
	int i = 0;
	Ogre::SceneNode* collided = nullptr;

	while (i < rayCasts_.size() && !found)
	{
		if (name == rayCasts_[i].rayName)
		{
			collided = rayCasts_[i].collision;
			found = true;
		}
		else i++;
	}

	return collided;
}

void Physics::setRaycastByName(const std::string name, btVector3 from, btVector3 to)
{
	bool found = false;
	int i = 0;

	while (i < rayCasts_.size() && !found)
	{
		if (name == rayCasts_[i].rayName)
		{
			rayCasts_[i].from = from;
			rayCasts_[i].to = to;
			rayCasts_[i].collision = nullptr;
			found = true;
		}
		else i++;
	}
}

void Physics::createRaycast(btVector3 from, btVector3 to, std::string name)
{
	rayCast r;
	r.from = from;
	r.to = to;
	r.rayName = name;

	rayCasts_.push_back(r);
}

Ogre::SceneNode * Physics::firstHitRaycast(btVector3 from, btVector3 to)
{
	btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	dynamicsWorld->rayTest(from, to, closestResults);

	Ogre::SceneNode *sceneNodeCollided = nullptr;

	if (closestResults.hasHit())
	{
		//This is the object the rayCast has collided with
		const btCollisionObject* collidedObj = closestResults.m_collisionObject;

		const btRigidBody* body = btRigidBody::upcast(collidedObj);

		sceneNodeCollided = static_cast<Ogre::SceneNode *>(body->getUserPointer());
		
		if (debug_)
		{
			btVector3 p = from.lerp(to, closestResults.m_closestHitFraction);
			debugDrawer::Instance()->drawCube(p, btVector3(1, 1, 1));
			debugDrawer::Instance()->drawLine(from, p, btVector3(1, 0, 0));
		}
	}
	else
	{

		if (debug_)
		{
			debugDrawer::Instance()->drawLine(from, to, btVector3(0, 1, 0));
		}
	}

	return sceneNodeCollided;
}

Ogre::SceneNode * Physics::castRays()
{

	Ogre::SceneNode * sceneNodeCollided = nullptr;
	///step the simulation
	if (dynamicsWorld)
	{
		dynamicsWorld->updateAabbs();
		dynamicsWorld->computeOverlappingPairs();

		for (int i = 0; i < rayCasts_.size(); i++)
		{
			rayCasts_[i].collision = firstHitRaycast(rayCasts_[i].from, rayCasts_[i].to);
		}
	}

	return sceneNodeCollided;
}