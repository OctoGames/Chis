#include <vector>
#include <map>

//Bullet Includes
#include <btBulletDynamicsCommon.h>

class Physics {
public:
	Physics();
	~Physics();

private:
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;

	void init();
};