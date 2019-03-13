#include <vector>
#include <map>

#include <Ogre.h>

#include <btBulletDynamicsCommon.h>

#include "Transform.h"

class Physics {

public:
	
	static Physics* Instance();

	inline void addToPhysicWorld(btRigidBody * body) { dynamicsWorld->addRigidBody(body); numberOfRigidBodies_++; };
	inline btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; };
	inline std::vector<btCollisionShape *> getCollisionShapes() { return collisionShapes; };
	inline void trackRigidBodyWithName(btRigidBody* b, std::string n) { physicsAccessors.insert(std::pair<std::string, btRigidBody*>(n, b)); };

	bool update();

	void createRigidBody(Ogre::SceneNode * node, double mass, std::string name);

	btRigidBody* getRigidBodyByName(std::string name);

private:

	Physics();
	~Physics();

	static Physics* instance_;

	int numberOfRigidBodies_;

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;
};