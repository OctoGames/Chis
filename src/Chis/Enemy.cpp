#include "Enemy.h"

#include "AudioSource.h"
#include "GameManager.h"
#include "Canvas.h"

std::string Enemy::name_ = "Enemy";

Enemy::Enemy() :
	health_(100.0f),
	damage_(10.0f),
	score_(100.0f)
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
	if (other->getTag() == "bullet")
	{
		damage(25);	// damage attribute from bullet

		AudioSource* as = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
		if (as) as->play();
	}
}

void Enemy::update()
{
	if (health_ <= 0)
	{
		Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
		if (canvas) canvas->updateScore(score_);

		EntityComponentManager::Instance()->destroy(gameObject());
	}
	
	// The enemy has a list of int numbers representing nodes (check AISystem)
	// In each tick, we set the orientation of the transform to the next node
	// and with the direction, we can set the linear velocity of the enemy's rigidbody
	// If we reach a node, we'll set as a destination the next one, and so on
	// If we collide with the Cheese (last node), we destroy the gameObject
}
