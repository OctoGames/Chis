#include <vector>
#include <map>

#include <btBulletDynamicsCommon.h>

#include <Ogre.h>

class Physics {

public:
	Physics();
	~Physics();
	inline void addToPhysicWorld(btRigidBody * body) { dynamicsWorld->addRigidBody(body); };
	btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; };
	inline std::vector<btCollisionShape *> getCollisionShapes() { return collisionShapes; };
	//void trackRigidBodyWithName(btRigidBody* b, std::string n) { physicsAccessors.insert(n, b); };

	bool frameStarted(const Ogre::FrameEvent &evt);

private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;
};