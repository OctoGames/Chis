#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include <iostream>
#include <vector>
#include <map>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

#include "EntityComponentManager.h"
#include "DebugDrawer.h"
#include "MeshStrider.h"

struct RaycastHit 
{
	GameObject* gameObject;
	btVector3 normal;
	btVector3 point;
	btScalar distance;
};

//Collider shape, created by the DebugDrawer
struct DebugObject
{
	enum DebugObjectType { DEBUG_BOX, DEBUG_SPHERE, DEBUG_LINE };
	DebugObjectType type;
	btVector3 debugColor;
	btVector3 dimension;
	btVector3 from;
	btVector3 to;
	float radius;
};

class Physics 
{
public:
	static Physics* Instance();

	void init();
	void update(float deltaTime);
	void close();

	void removeRigidbody(btRigidBody* rigidbody);
	inline void addRigidBody(const std::string& rigidBodyName, btRigidBody* rigidBody) { rigidBodies_[rigidBodyName] = rigidBody; };
	inline void addDebugObject(const std::string& debugObjectName, DebugObject* debugObject) { debugObjects_[debugObjectName] = debugObject; }
	inline btRigidBody* getRigidBody(const std::string& rigidBodyName) { return rigidBodies_[rigidBodyName]; }
	inline DebugObject* getDebugObject(const std::string& debugObjectName) { return debugObjects_[debugObjectName]; }

	btRigidBody* createRigidBody(GameObject * gameObject, float mass, btVector3 dimension);
	btRigidBody* createRigidBody(GameObject * gameObject, float mass, float radius);
	btRigidBody* createRigidBody(GameObject * gameObject, float mass, Ogre::MeshPtr mesh);

	bool raycast(btVector3 from, btVector3 to, RaycastHit* hit = nullptr);
	bool raycast(btVector3 from, btVector3 normal, float maxLength, RaycastHit* hit = nullptr);

	void toggleDebug() { if (debugMode_) drawDebugObjects_ = !drawDebugObjects_; }
	void setDebugMode(bool active) { debugMode_ = active; };
	void toggleDebugMode();

private:
	Physics();
	~Physics();

	void detectCollisions();
	btTransform* createTransform(GameObject * gameObject);
	DebugObject* createDebugObject(float radius);
	DebugObject* createDebugObject(btVector3 dimension);
	DebugObject* createDebugObject(btVector3 from, btVector3 to);

	static Physics* instance_;

	bool debugMode_;
	bool drawDebugObjects_;

	DebugDrawer* debugDrawer_;
	btCollisionDispatcher* dispatcher_;
	btDiscreteDynamicsWorld* dynamicsWorld_;
	btBroadphaseInterface* overlappingPairCache_;
	btSequentialImpulseConstraintSolver* solver_;
	btDefaultCollisionConfiguration* collisionConfiguration_;

	std::map<std::string, btRigidBody*> rigidBodies_;
	std::map<std::string, DebugObject*> debugObjects_;
};

#endif // !__PHYSICS_H__