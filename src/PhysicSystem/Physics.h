#include <vector>
#include <map>

#include <Ogre.h>

#include <btBulletDynamicsCommon.h>

#include "Transform.h"

#include "debugDrawer.h"

class Physics {

public:
	
	static Physics* Instance();

	inline void addToPhysicWorld(btRigidBody * body) { dynamicsWorld->addRigidBody(body); numberOfRigidBodies_++; };
	inline btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; };
	inline std::vector<btCollisionShape *> getCollisionShapes() { return collisionShapes; };
	inline void trackRigidBodyWithName(btRigidBody* b, std::string n) { physicsAccessors.insert(std::pair<std::string, btRigidBody*>(n, b)); };

	bool update();

	void createRigidBody(Ogre::SceneNode * node, double mass, btVector3 scale, std::string name);

	btRigidBody* getRigidBodyByName(std::string name);

	inline void setDebugMode(bool db) { debug_ = db; };

	inline void toggleDebugMode() { debug_ = !debug_; if (!debug_) debugDrawer::Instance()->resetLineNumber();};

private:

	struct debugObjectsPropierties {
		Ogre::SceneNode* node;
		btVector3 scale;
	};

	Physics();
	~Physics();

	void debugMode();

	bool debug_;

	debugDrawer* dbg_drawer;

	static Physics* instance_;

	int numberOfRigidBodies_;

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;

	std::vector<debugObjectsPropierties> debugObjects;
};