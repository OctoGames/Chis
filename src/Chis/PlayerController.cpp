#include "PlayerController.h"

#include "RigidBody.h"

std::string PlayerController::name_ = "PlayerController";

PlayerController::PlayerController() :
	rigidbody_(nullptr),
	maxSpeed_(0.0f),
	velocity_(Ogre::Vector3::ZERO),
	moveForwardsKey_(OIS::KC_W),
	moveBackwardsKey_(OIS::KC_S),
	moveLeftKey_(OIS::KC_A),
	moveRightKey_(OIS::KC_D),
	fastMoveKey_(OIS::KC_LSHIFT),
	goingForward_(false),
	goingBack_(false),
	goingLeft_(false),
	goingRight_(false),
	fastMove_(false)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_pc"); if (it != params.end()) enabled_ = params.at("enabled_pc").b;
	it = params.find("max_speed"); if (it != params.end()) maxSpeed_ = params.at("max_speed").f;
}

Component * PlayerController::clone()
{
	PlayerController* clonedComponent = new PlayerController();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->maxSpeed_ = this->maxSpeed_;

	clonedComponent->moveForwardsKey_ = this->moveForwardsKey_;
	clonedComponent->moveBackwardsKey_ = this->moveBackwardsKey_;
	clonedComponent->moveLeftKey_ = this->moveLeftKey_;
	clonedComponent->moveRightKey_ = this->moveRightKey_;
	clonedComponent->fastMoveKey_ = this->fastMoveKey_;

	return clonedComponent;
}

void PlayerController::init()
{
	InputManager::Instance()->addKeyListener(this, "PlayerController");
	InputManager::Instance()->addMouseListener(this, "PlayerController");

	setEnabled(enabled_);
}

void PlayerController::start()
{
	RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(gameObject(), "RigidBody"));
	rigidbody_ = rb->rigidbody();
	rigidbody_->setLinearFactor(btVector3(1, 0, 1));
	rigidbody_->setAngularFactor(btVector3(0, 1, 0));
}

void PlayerController::fixedUpdate()
{
	float deltaTime = RenderManager::Instance()->time()->deltaTime();
	setPlayerAcceleration(deltaTime);
	rigidbody_->activate();

	if (velocity_ != Ogre::Vector3::ZERO)
	{
		btVector3 v(velocity_.x, velocity_.y, velocity_.z);
		rigidbody_->setLinearVelocity(v * deltaTime);
	}
}

void PlayerController::onCollision(GameObject* other)
{
	if (other->getName() != "mesa")
	{
		rigidbody_->setLinearVelocity(btVector3(0, 0, 0));
		rigidbody_->setAngularVelocity(btVector3(0, 0, 0));
	}
}

bool PlayerController::keyPressed(const OIS::KeyEvent & e)
{
	if (e.key == moveForwardsKey_) goingForward_ = true;
	else if (e.key == moveBackwardsKey_) goingBack_ = true;
	else if (e.key == moveLeftKey_) goingLeft_ = true;
	else if (e.key == moveRightKey_) goingRight_ = true;

	return true;
}

bool PlayerController::keyReleased(const OIS::KeyEvent & e)
{
	if (e.key == moveForwardsKey_) goingForward_ = false;
	else if (e.key == moveBackwardsKey_) goingBack_ = false;
	else if (e.key == moveLeftKey_) goingLeft_ = false;
	else if (e.key == moveRightKey_) goingRight_ = false;

	return true;
}

void PlayerController::setPlayerAcceleration(float deltaTime)
{
	Ogre::Vector3 accel = Ogre::Vector3::ZERO;
	Ogre::Matrix3 axes = gameObject()->transform()->getLocalAxes();
	if (goingForward_) accel -= axes.GetColumn(2);
	if (goingBack_) accel += axes.GetColumn(2);
	if (goingRight_) accel += axes.GetColumn(0);
	if (goingLeft_) accel -= axes.GetColumn(0);

	Ogre::Real topSpeed = fastMove_ ? maxSpeed_ * 20 : maxSpeed_;
	if (accel.squaredLength() != 0)
	{
		accel.normalise();
		velocity_ += accel * topSpeed * deltaTime * 10;
	}
	else
	{
		velocity_ -= velocity_ * deltaTime * 10;
	}

	setPlayerVelocity(topSpeed);
}

void PlayerController::setPlayerVelocity(Ogre::Real topSpeed)
{
	Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

	if (velocity_.squaredLength() > topSpeed * topSpeed)
	{
		velocity_.normalise();
		velocity_ *= topSpeed;
	}
	else if (velocity_.squaredLength() < tooSmall * tooSmall)
	{
		velocity_ = Ogre::Vector3::ZERO;
	}

	velocity_.y = 0.0f;
}
