#include <vector>
#include <map>

#include <Ogre.h>
//#include "Transform.h"

#include <btBulletDynamicsCommon.h>

#include "debugDrawer.h"

class Physics {

public:
	
	static Physics* Instance();

	inline void addToPhysicWorld(btRigidBody * body) { dynamicsWorld->addRigidBody(body); numberOfRigidBodies_++; };
	inline btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; };
	inline std::vector<btCollisionShape *> getCollisionShapes() { return collisionShapes; };
	inline void trackRigidBodyWithName(btRigidBody* b, std::string n) { physicsAccessors.insert(std::pair<std::string, btRigidBody*>(n, b)); };

	bool update();

	btRigidBody* getRigidBodyByName(std::string name);

	inline void setDebugMode(bool db) { debug_ = db; };

	inline void toggleDebugMode() { debug_ = !debug_; if (!debug_) debugDrawer::Instance()->resetLineNumber();};


	void createBoxRigidBody(Ogre::SceneNode * node, double mass, btVector3 scale, std::string name);
	void createSphereRididBody(Ogre::SceneNode * node, double mass, double radious, std::string name);

private:

	struct debugObjectsPropierties {
		Ogre::SceneNode* node;
		btVector3 scale;
		double radious;
		std::string type;
	};

	Physics();
	~Physics();

	btTransform setTransform(Ogre::SceneNode* node);
	void setRigidBody(btCollisionShape* shape, double mass, std::string name, Ogre::SceneNode* node, btTransform transform);
	void setDebugObject(Ogre::SceneNode * node, std::string type, double radious, btVector3 scale);

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