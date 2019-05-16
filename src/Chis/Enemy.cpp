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
	speed_(1000.0f)
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
	it = params.find("speed"); if (it != params.end()) speed_ = params.at("speed").f;
}

Component * Enemy::clone()
{
	Enemy* clonedComponent = new Enemy();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->health_ = this->health_;
	clonedComponent->damage_ = this->damage_;
	clonedComponent->score_ = this->score_;
	clonedComponent->speed_ = this->speed_;

	return clonedComponent;
}

void Enemy::init()
{
	gameObject()->transform()->setScale(gameObject()->transform()->getScale() * 100);
	RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(gameObject(), "RigidBody"));
	if (rb)
	{
		rb->rigidbody()->setLinearFactor(btVector3(1, 0, 1));
		rb->rigidbody()->setAngularFactor(btVector3(0, 1, 0));
	}
	setEnabled(enabled_);
}

void Enemy::onCollision(GameObject * other)
{
	if (other->getName() != "mesa")
	{
		RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(gameObject(), "RigidBody"));
		if (rb) rb->rigidbody()->setLinearVelocity(btVector3(0, 0, 0));
		if (rb) rb->rigidbody()->setAngularVelocity(btVector3(0, 0, 0));
	}
}

void Enemy::start()
{
}

void Enemy::update()
{
	if (health_ <= 0)
	{
		Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
		if (canvas) canvas->updateScore(score_);

		EntityComponentManager::Instance()->destroy(gameObject());
	}

	if (currentDestination_ == nullptr)
	{
		if (!path_.empty())
		{
			currentDestination_ = path_.front();
			path_.pop();
		}
	}

	if (currentDestination_)
	{
		if (currentDestination_->getPosition() == gameObject()->transform()->getPosition())
		{
			if (!path_.empty())
			{
				currentDestination_ = path_.front();
				path_.pop();
			}
		}

		Ogre::Vector3 dir = currentDestination_->getPosition() - gameObject()->transform()->getPosition();
		dir.normalise();

		btVector3 direction(dir.x, dir.y, dir.z);
		RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(gameObject(), "RigidBody"));
		if (rb) rb->rigidbody()->setLinearVelocity(direction * speed_);
	}
}

void Enemy::receivePath(const std::list<int>& path)
{
	for (int i : path)
	{
		path_.push(Node::nodes_[i]);
	}
}
