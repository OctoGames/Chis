#include "AmmoSpawner.h"

#include "Node.h"
#include "Ammo.h"


std::string AmmoSpawner::name_ = "AmmoSpawner";

AmmoSpawner::AmmoSpawner() :
	spawnTime_(15000),
	spawnTimer_(nullptr)
{
}

AmmoSpawner::~AmmoSpawner()
{
}

void AmmoSpawner::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_es"); if (it != params.end()) enabled_ = params.at("enabled_es").b;
	it = params.find("spawn_time"); if (it != params.end()) spawnTime_ = params.at("spawn_time").b;
}

Component * AmmoSpawner::clone()
{
	AmmoSpawner* clonedComponent = new AmmoSpawner();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->spawnTime_= this->spawnTime_;

	return clonedComponent;
}

void AmmoSpawner::init()
{
	spawnTimer_ = new Ogre::Timer(); 
	spawnTimer_->reset();
	setEnabled(enabled_);
}

void AmmoSpawner::update()
{
	if (spawnTimer_->getMilliseconds() >= spawnTime_) spawnAmmo();
}

void AmmoSpawner::spawnAmmo()
{
	std::cout << "Spawning beginning...\n";
	std::list<GameObject*> nodeObjects = EntityComponentManager::Instance()->findGameObjectsWithTag("node");
	int nodeIndex = rand() % 47;

	std::cout << "Spawn node:" << nodeIndex << "\n";
	Node* node = nullptr;
	for (GameObject* obj : nodeObjects)
	{
		Node* nodeComponent = static_cast<Node*>(EntityComponentManager::Instance()->getComponent(obj, "Node"));
		if (nodeComponent && nodeComponent->getID() == nodeIndex) { node = nodeComponent; break; }
	}

	if (node)
	{
		std::cout << "Spawning ammo...\n";
		Ogre::Vector3 incr = node->gameObject()->transform()->getPosition() + Ogre::Vector3(0, 50, 0);
		GameObject* box = EntityComponentManager::Instance()->instantiate("Ammo", incr);

		spawnTimer_->reset();
	}
}
