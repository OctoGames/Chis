#include "Physics.h"

Physics* Physics::instance_ = nullptr;


// MANAGER FUNCTIONS-----------------------------------------------------------

Physics* Physics::Instance()
{
	if (instance_ == nullptr) instance_ = new Physics();
	return instance_;
}

Physics::Physics() :
	debugMode_(false),
	drawDebugObjects_(true),
	debugDrawer_(nullptr),
	dispatcher_(nullptr),
	dynamicsWorld_(nullptr),
	overlappingPairCache_(nullptr),
	solver_(nullptr),
	collisionConfiguration_(nullptr)
{
}

Physics::~Physics()
{

}

void Physics::init()
{
	collisionConfiguration_ = new btDefaultCollisionConfiguration();
	dispatcher_ = new btCollisionDispatcher(collisionConfiguration_);
	overlappingPairCache_ = new btDbvtBroadphase();
	solver_ = new btSequentialImpulseConstraintSolver();
	dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_, overlappingPairCache_, solver_, collisionConfiguration_);

	dynamicsWorld_->setGravity(btVector3(0, -9.8, 0));

#if _DEBUG
	// If true the framerate drops drastically
	setDebugMode(false);
#else
	setDebugMode(false);
#endif
}

void Physics::update(float deltaTime)
{
	// Update the dynamic world
	// This automatically performs collision detection and physics simulation
	// It also updates the world transform for all the active objects
	dynamicsWorld_->stepSimulation(deltaTime);

	// Link the data in Physics and Render engines
	for (auto a : rigidBodies_)
	{
		btRigidBody* rb = a.second;
		if (static_cast<GameObject*>(rb->getUserPointer())->isActive())
		{
			Ogre::SceneNode* n = static_cast<GameObject*>(rb->getUserPointer())->transform();

			btVector3 rbPos = rb->getCenterOfMassPosition();
			btQuaternion rbRot = rb->getOrientation();
			Ogre::Vector3 nPos(rbPos.x(), rbPos.y(), rbPos.z());
			Ogre::Quaternion nRot(rbRot.w(), rbRot.x(), rbRot.y(), rbRot.z());
			n->setPosition(nPos);
			n->setOrientation(nRot);
		}
	}

	// Proccess all the collisions
	detectCollisions();

	// Draw the Debug Objects
	if (debugMode_)
	{
		DebugDrawer::Instance()->resetLineNumber();

		for (auto a : debugObjects_)
		{
			DebugObject* dbg = a.second;

			if (dbg->type == DebugObject::DEBUG_LINE)
			{
				btVector3 color;
				
				DebugDrawer::Instance()->drawLine(dbg->from, dbg->to, btVector3(1, 0, 0));
			}
			else
			{
				btRigidBody* rb = rigidBodies_[a.first];
				Ogre::SceneNode* n = static_cast<GameObject*>(rb->getUserPointer())->transform();
				n->setVisible(drawDebugObjects_);

				if (dbg->type == DebugObject::DEBUG_BOX)
					DebugDrawer::Instance()->drawCube(rb->getCenterOfMassPosition(), dbg->dimension);
				else if (dbg->type == DebugObject::DEBUG_SPHERE)
					DebugDrawer::Instance()->drawSphere(rb->getCenterOfMassPosition(), dbg->radius);
			}
		}
	}
}

void Physics::close()
{
	for (auto a : debugObjects_)
	{
		delete a.second;
	}
	debugObjects_.clear();

	for (int i = dynamicsWorld_->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld_->getCollisionObjectArray()[i];
		dynamicsWorld_->removeCollisionObject(obj);
		btCollisionShape* shape = obj->getCollisionShape();
		delete shape;
		delete obj;
	}
	rigidBodies_.clear();

	delete dynamicsWorld_;
	delete solver_;
	delete overlappingPairCache_;
	delete dispatcher_;
	delete collisionConfiguration_;
}

void Physics::removeRigidbody(btRigidBody* rigidbody)
{
	std::string id = static_cast<GameObject*>(rigidbody->getUserPointer())->getGameObjectID();
	rigidBodies_.erase("rb" + id);
	
	delete debugObjects_["rb" + id];
	debugObjects_.erase("rb" + id);

	dynamicsWorld_->removeCollisionObject(rigidbody);
	btCollisionShape* shape = rigidbody->getCollisionShape();
	delete shape;
	delete rigidbody;
}


// RIGIDBODY FUNCTIONS---------------------------------------------------------

btRigidBody * Physics::createRigidBody(GameObject * gameObject, float mass, btVector3 dimension)
{
	// Define the initial Transform and Motion State for the RigidBody
	btTransform* rbTransform = createTransform(gameObject);
	btDefaultMotionState* rbMotionState = new btDefaultMotionState(*rbTransform);
	DebugObject* debugObject = createDebugObject(dimension);

	// Define the Shape, Mass and Local Inertia for the RigidBody
	btScalar rbMass(mass); btVector3 rbLocalInertia(0.0, 0.0, 0.0);
	btCollisionShape* rbShape = new btBoxShape(dimension);
	rbShape->calculateLocalInertia(rbMass, rbLocalInertia);

	// Create the RigidBody with all the previous data and store a reference to the GameObject
	btRigidBody::btRigidBodyConstructionInfo rbInfo(rbMass, rbMotionState, rbShape, rbLocalInertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);
	rigidBody->setUserPointer(gameObject);

	// Register the new RigidBody in the Physics Manager data structures
	addRigidBody("rb" + gameObject->getGameObjectID(), rigidBody);
	addDebugObject("rb" + gameObject->getGameObjectID(), debugObject);
	dynamicsWorld_->addRigidBody(rigidBody);
	return rigidBody;
}

btRigidBody * Physics::createRigidBody(GameObject * gameObject, float mass, float radius)
{
	// Define the initial Transform, Motion State and Debug Object for the RigidBody
	btTransform* rbTransform = createTransform(gameObject);
	btDefaultMotionState* rbMotionState = new btDefaultMotionState(*rbTransform);
	DebugObject* debugObject = createDebugObject(radius);

	// Define the Shape, Mass and Local Inertia for the RigidBody
	btScalar rbMass(mass); btVector3 rbLocalInertia(0.0, 0.0, 0.0);
	btCollisionShape* rbShape = new btSphereShape(radius);
	rbShape->calculateLocalInertia(rbMass, rbLocalInertia);

	// Create the RigidBody with all the previous data and store a reference to the GameObject
	btRigidBody::btRigidBodyConstructionInfo rbInfo(rbMass, rbMotionState, rbShape, rbLocalInertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);
	rigidBody->setUserPointer(gameObject);

	// Register the new RigidBody in the Physics Manager data structures
	addRigidBody("rb" + gameObject->getGameObjectID(), rigidBody);
	addDebugObject("rb" + gameObject->getGameObjectID(), debugObject);

	dynamicsWorld_->addRigidBody(rigidBody);
	return rigidBody;
}

btRigidBody * Physics::createRigidBody(GameObject * gameObject, float mass, Ogre::MeshPtr mesh)
{
	// Define the initial Transform, Motion State and Debug Object for the RigidBody
	btTransform* rbTransform = createTransform(gameObject);
	btDefaultMotionState* rbMotionState = new btDefaultMotionState(*rbTransform);
	//DebugObject* debugObject = createDebugObject(radius);

	// Define the Shape, Mass and Local Inertia for the RigidBody
	btScalar rbMass(mass); btVector3 rbLocalInertia(0.0, 0.0, 0.0);
	MeshStrider* Strider = new MeshStrider(mesh.getPointer());
	btCollisionShape* rbShape = new btBvhTriangleMeshShape(Strider, true, true);
	rbShape->calculateLocalInertia(rbMass, rbLocalInertia);

	// Create the RigidBody with all the previous data and store a reference to the GameObject
	btRigidBody::btRigidBodyConstructionInfo rbInfo(rbMass, rbMotionState, rbShape, rbLocalInertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);
	rigidBody->setUserPointer(gameObject);

	// Register the new RigidBody in the Physics Manager data structures
	addRigidBody("rb" + gameObject->getGameObjectID(), rigidBody);
	//addDebugObject("rb" + gameObject->getGameObjectID(), debugObject);

	dynamicsWorld_->addRigidBody(rigidBody);
	return rigidBody;
}

btTransform * Physics::createTransform(GameObject * gameObject)
{
	Ogre::SceneNode * node = gameObject->transform();
	Ogre::Vector3 pos = node->getPosition();
	Ogre::Quaternion rot = node->getOrientation();

	btVector3 p = btVector3(pos.x, pos.y, pos.z);
	btQuaternion q = btQuaternion(rot.x, rot.y, rot.z, rot.w);

	return new btTransform(q, p);
}

void Physics::detectCollisions()
{
	for (int i = 0; i < dispatcher_->getNumManifolds(); i++)
	{
		// Get the collision manifold to access both colliding objects
		btPersistentManifold* contactManifold = dispatcher_->getManifoldByIndexInternal(i);

		// Register the first colliding object
		const btCollisionObject* objectA = contactManifold->getBody0();
		const btRigidBody* rigidBodyA = btRigidBody::upcast(objectA);

		// Register the second colliding object
		const btCollisionObject* objectB = contactManifold->getBody1();
		const btRigidBody* rigidBodyB = btRigidBody::upcast(objectB);

		// Access the GameObjects associated to both colliding objects
		GameObject* gameObjectA = static_cast<GameObject*>(rigidBodyA->getUserPointer());
		GameObject* gameObjectB = static_cast<GameObject*>(rigidBodyB->getUserPointer());

		// Call onCollision method for each component of both GameObjects
		for (Component* c : EntityComponentManager::Instance()->getComponents(gameObjectA->getGameObjectID()))
			c->onCollision(gameObjectB);
		for (Component* c : EntityComponentManager::Instance()->getComponents(gameObjectB->getGameObjectID()))
			c->onCollision(gameObjectA);
	}
}



// RAYCAST FUNCTIONS-----------------------------------------------------------

bool Physics::raycast(btVector3 from, btVector3 to, RaycastHit* hit)
{
	btCollisionWorld::ClosestRayResultCallback ray(from, to);

	dynamicsWorld_->updateAabbs();
	dynamicsWorld_->computeOverlappingPairs();
	dynamicsWorld_->rayTest(from, to, ray);

	if (ray.hasHit())
	{
		if (hit)
		{
			const btRigidBody* rb = btRigidBody::upcast(ray.m_collisionObject);
			hit->gameObject = static_cast<GameObject*>(rb->getUserPointer());
			hit->normal = ray.m_hitNormalWorld;
			hit->point = ray.m_hitPointWorld;
			hit->distance = ray.m_rayFromWorld.distance(ray.m_hitPointWorld);
		}

		DebugObject* dbg = createDebugObject(ray.m_rayFromWorld, ray.m_hitPointWorld);
		dbg->debugColor = btVector3(0, 1, 0);
		addDebugObject("Raycast", dbg);
		return true;
	}

	DebugObject* dbg = createDebugObject(ray.m_rayFromWorld, ray.m_rayToWorld);
	dbg->debugColor = btVector3(1, 0, 0);
	addDebugObject("Raycast", dbg);
	return false;
}

bool Physics::raycast(btVector3 from, btVector3 normal, float maxLength, RaycastHit * hit)
{
	btVector3 to = normal * maxLength;
	btCollisionWorld::ClosestRayResultCallback ray(from, to);

	dynamicsWorld_->updateAabbs();
	dynamicsWorld_->computeOverlappingPairs();
	dynamicsWorld_->rayTest(from, to, ray);

	if (ray.hasHit())
	{
		if (hit)
		{
			const btRigidBody* rb = btRigidBody::upcast(ray.m_collisionObject);
			hit->gameObject = static_cast<GameObject*>(rb->getUserPointer());
			hit->normal = ray.m_hitNormalWorld;
			hit->point = ray.m_hitPointWorld;
			hit->distance = ray.m_rayFromWorld.distance(ray.m_hitPointWorld);
		}
		
		DebugObject* dbg = createDebugObject(ray.m_rayFromWorld, ray.m_hitPointWorld);
		dbg->debugColor = btVector3(0, 1, 0);
		addDebugObject("Raycast", dbg);
		return true;
	}

	DebugObject* dbg = createDebugObject(ray.m_rayFromWorld, ray.m_rayToWorld);
	dbg->debugColor = btVector3(1, 0, 0);
	addDebugObject("Raycast", dbg);
	return false;
}



// DEBUG FUNCTIONS-------------------------------------------------------------

DebugObject * Physics::createDebugObject(float radius)
{
	DebugObject* debugObject = new DebugObject();
	debugObject->radius = radius;
	debugObject->type = DebugObject::DEBUG_SPHERE;
	debugObject->debugColor = btVector3(0, 1, 0);
	return debugObject;
}

DebugObject * Physics::createDebugObject(btVector3 dimension)
{
	DebugObject* debugObject = new DebugObject();
	debugObject->dimension = dimension;
	debugObject->type = DebugObject::DEBUG_BOX;
	debugObject->debugColor = btVector3(0, 1, 0);
	return debugObject;
}

DebugObject * Physics::createDebugObject(btVector3 from, btVector3 to)
{
	DebugObject* debugObject = new DebugObject();
	debugObject->from = from;
	debugObject->to = to;
	debugObject->type = DebugObject::DEBUG_LINE;
	return debugObject;
}

void Physics::toggleDebugMode()
{
	debugMode_ = !debugMode_;

	if (!debugMode_)
	{
		DebugDrawer::Instance()->resetLineNumber();
		for (auto a : rigidBodies_)
			static_cast<GameObject*>(a.second->getUserPointer())->transform()->setVisible(true);
	}
}