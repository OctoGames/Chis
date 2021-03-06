#include "EnemySpawner.h"

#include "Node.h"
#include "Enemy.h"


std::string EnemySpawner::name_ = "EnemySpawner";
int EnemySpawner::numEnemies_ = 0;

EnemySpawner::EnemySpawner() :
	spawnTime_(5000),
	spawnTimer_(nullptr)
{
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_es"); if (it != params.end()) enabled_ = params.at("enabled_es").b;
	it = params.find("spawn_time"); if (it != params.end()) spawnTime_ = params.at("spawn_time").b;
}

Component * EnemySpawner::clone()
{
	EnemySpawner* clonedComponent = new EnemySpawner();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->spawnTime_= this->spawnTime_;

	return clonedComponent;
}

void EnemySpawner::init()
{
	spawnTimer_ = new Ogre::Timer(); 
	spawnTimer_->reset();
	setEnabled(enabled_);
}

void EnemySpawner::update()
{
	if (spawnTimer_->getMilliseconds() >= spawnTime_) spawnEnemy();
}

void EnemySpawner::spawnEnemy()
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
		std::cout << "Spawning enemy...\n";
		Ogre::Vector3 incr = node->gameObject()->transform()->getPosition() + Ogre::Vector3(0, 50, 0);
		GameObject* enemy = EntityComponentManager::Instance()->instantiate("Mouse", incr);
		numEnemies_++;

		std::list<int> path = AIManager::Instance()->getPath(nodeIndex, 0);
		Enemy* enemyComponent = static_cast<Enemy*>(EntityComponentManager::Instance()->getComponent(enemy, "Enemy"));
		if (enemyComponent) enemyComponent->receivePath(path);

		spawnTimer_->reset();
	}
}
