#include "Enemy.h"

#include "AudioSource.h"
#include "RigidBody.h"
#include "Canvas.h"
#include "Node.h"

std::string Enemy::name_ = "Enemy";

Enemy::Enemy() :
	health_(100.0f),
	damage_(10.0f),
	score_(100.0f),
	speed_(1000.0f),
	currentNode_(0),
	numNodes_(0),
	destination_(nullptr)
{
}

Enemy::~Enemy()
{
}

void Enemy::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_e"); if (it != params.end()) enabled_ = params.at("enabled_e").b;
	it = params.find("health"); if (it != params.end()) health_ = params.at("health").f;
	it = params.find("damage"); if (it != params.end()) damage_ = params.at("damage").f;
	it = params.find("score"); if (it != params.end()) score_ = params.at("score").f;
}

Component * Enemy::clone()
{
	Enemy* clonedComponent = new Enemy();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->health_ = this->health_;
	clonedComponent->damage_ = this->damage_;
	clonedComponent->score_  = this->score_;

	return clonedComponent;
}

void Enemy::init()
{
	setEnabled(enabled_);
}

void Enemy::onCollision(GameObject * other)
{
}

void Enemy::start()
{
	// Pedir camino al motor de IA
	// Nos devuelve una lista de int n
	// Rellenamos el vector path con Node::nodes_[n]
	// Asignamos destination_
}

void Enemy::update()
{
	if (health_ <= 0)
	{
		Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
		if (canvas) canvas->updateScore(score_);

		EntityComponentManager::Instance()->destroy(gameObject());
	}
	
	//if (gameObject()->transform()->getPosition() != destination_->getPosition()) 
	//{
	//	gameObject()->transform()->lookAt(destination_->getPosition(), Ogre::Node::TS_WORLD);
	//	Ogre::Vector3 dir = destination_->getPosition() - gameObject()->transform()->getPosition();
	//	dir.normalise();

	//	btVector3 direction(dir.x, dir.y, dir.z);

	//	RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(bullet, "RigidBody"));
	//	if (rb) rb->rigidbody()->setLinearVelocity(direction * speed_);
	//}
	//else
	//{
	//	currentNode_++;
	//	if (currentNode_ > numNodes_)
	//	{
	//		destination_ = Node::nodes_[path_[currentNode_]]->gameObject()->transform();
	//	}
	//}
}
