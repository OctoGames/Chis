#include "Cheese.h"

#include "AudioSource.h"
#include "Canvas.h"
#include "Enemy.h"


std::string Cheese::name_ = "Cheese";

Cheese::Cheese() :
	cheesyness_(100.0f)
{
}

Cheese::~Cheese()
{
}

void Cheese::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_ch"); if (it != params.end()) enabled_ = params.at("enabled_ch").b;
	it = params.find("health"); if (it != params.end()) cheesyness_ = params.at("health").f;
}

Component * Cheese::clone()
{
	Cheese* clonedComponent = new Cheese();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->cheesyness_  = this->cheesyness_;

	return clonedComponent;
}

void Cheese::init()
{
	setEnabled(enabled_);
}

void Cheese::onCollision(GameObject * other)
{
	if (other->getTag() == "enemy")	// enemy_bullet
	{
		Enemy* enemy = static_cast<Enemy*>(EntityComponentManager::Instance()->getComponent(other, "Enemy"));
		if (enemy) damage(enemy->getDamage());

		Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
		if (canvas) canvas->updateCheese(cheesyness_);

		AudioSource* as = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
		if (as) as->play();
		
		EntityComponentManager::Instance()->destroy(other);
	}
}

void Cheese::update()
{
}
