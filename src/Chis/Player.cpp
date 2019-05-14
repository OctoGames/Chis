#include "Player.h"

#include "GameManager.h"
#include "Canvas.h"
#include "Ammo.h"

std::string Player::name_ = "Player";

Player::Player() :
	life_(100),
	bullets_(20),
	invulnerable_(false),
	invulnerabilityTimer_(nullptr)
{
}

Player::~Player()
{
}

void Player::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_p"); if (it != params.end()) enabled_ = params.at("enabled_p").b;
}

Component * Player::clone()
{
	Player* clonedComponent = new Player();

	clonedComponent->enabled_ = this->enabled_;

	return clonedComponent;
}

void Player::init()
{
	invulnerabilityTimer_ = new Ogre::Timer();
	invulnerabilityTimer_->reset();
	setEnabled(enabled_);
}

void Player::onCollision(GameObject * other)
{
	if (!invulnerable_ && other->getTag() == "enemy")
	{
		damage(10);
		std::cout << "DAMAGE: 10\t LIFE: " << life_ << "\n";
		Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
		if (canvas) canvas->updateLife(life_);

		invulnerable_ = true;
		invulnerabilityTimer_->reset();
	}
	else if (other->getTag() == "ammo")
	{
		Ammo* ammo = static_cast<Ammo*>(EntityComponentManager::Instance()->getComponent(other, "Ammo"));
		if (ammo)
		{
			bullets_ += ammo->getNumBullets();
			std::cout << "Picked up bullets. You have " << bullets_ << " bullets.\n";
		}
	}
}

void Player::update()
{
	if (invulnerable_ && invulnerabilityTimer_->getMilliseconds() > 2000)
	{
		invulnerable_ = false;
	}
}
