#include "Player.h"

#include "FirstPersonCamera.h"
#include "AudioSource.h"
#include "Canvas.h"
#include "Enemy.h"

std::string Player::name_ = "Player";

Player::Player() :
	health_(100.0f),
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
	it = params.find("health"); if (it != params.end()) health_ = params.at("health").f;
}

Component * Player::clone()
{
	Player* clonedComponent = new Player();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->health_  = this->health_;

	return clonedComponent;
}

void Player::init()
{
	Ogre::Viewport* vp = static_cast<FirstPersonCamera*>(EntityComponentManager::Instance()->getComponent(gameObject(), "FirstPersonCamera"))->getCamera()->getViewport();
	Ogre::CompositorManager::getSingleton().addCompositor(vp, "Luminance");
	invulnerabilityTimer_ = new Ogre::Timer();
	invulnerabilityTimer_->reset();
	setEnabled(enabled_);
}

void Player::onCollision(GameObject * other)
{
	if (!invulnerable_ && other->getTag() == "enemy")
	{
		Enemy* enemy = static_cast<Enemy*>(EntityComponentManager::Instance()->getComponent(other, "Enemy"));
		if (enemy) damage(enemy->getDamage());

		Canvas* canvas = static_cast<Canvas*>(EntityComponentManager::Instance()->getComponent("GameManager", "Canvas"));
		if (canvas) canvas->updateHealth(health_);

		AudioSource* as = static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"));
		if (as) as->play();
		
		Ogre::Viewport* vp = static_cast<FirstPersonCamera*>(EntityComponentManager::Instance()->getComponent(gameObject(), "FirstPersonCamera"))->getCamera()->getViewport();
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, "Luminance", true);

		invulnerable_ = true;
		invulnerabilityTimer_->reset();
	}
}

void Player::update()
{
	if (invulnerable_ && invulnerabilityTimer_->getMilliseconds() > 2000)
	{
		Ogre::Viewport* vp = static_cast<FirstPersonCamera*>(EntityComponentManager::Instance()->getComponent(gameObject(), "FirstPersonCamera"))->getCamera()->getViewport();
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, "Luminance", false);
		invulnerable_ = false;
	}
}
