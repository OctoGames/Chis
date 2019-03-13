#include <vector>
#include <map>

#include <Ogre.h>

#include <btBulletDynamicsCommon.h>

#include "MeshStrider.h"

class Physics {

public:
	Physics();
	~Physics();

	inline void addToPhysicWorld(btRigidBody * body) { dynamicsWorld->addRigidBody(body); numberOfRigidBodies_++; };
	inline btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; };
	inline std::vector<btCollisionShape *> getCollisionShapes() { return collisionShapes; };
	inline void trackRigidBodyWithName(btRigidBody* b, std::string n) { physicsAccessors.insert(std::pair<std::string, btRigidBody*>(n, b)); };

	bool update();

	void createRigidBody(btVector3 position, double mass, std::string meshName, Ogre::SceneNode* node);

	btRigidBody* getRigidBodyByName(std::string name);

private:

	int numberOfRigidBodies_;

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;
};