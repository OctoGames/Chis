#include "FirstPersonMovement.h"


std::string FirstPersonMovement::name_ = "FirstPersonMovement";


FirstPersonMovement::FirstPersonMovement() :
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

FirstPersonMovement::~FirstPersonMovement()
{
}

void FirstPersonMovement::load(const std::map<std::string, ValueType>& params)
{
	enabled_ = params.at("enabled_fpm").b;
	maxSpeed_ = params.at("max_speed").f;
	return;
}

Component * FirstPersonMovement::clone()
{
	FirstPersonMovement* clonedComponent = new FirstPersonMovement();

	clonedComponent->enabled_ = this->enabled_;


	clonedComponent->moveForwardsKey_ = this->moveForwardsKey_;
	clonedComponent->moveBackwardsKey_ = this->moveBackwardsKey_;
	clonedComponent->moveLeftKey_ = this->moveLeftKey_;
	clonedComponent->moveRightKey_ = this->moveRightKey_;
	clonedComponent->fastMoveKey_ = this->fastMoveKey_;

	return clonedComponent;
}

void FirstPersonMovement::init()
{
	InputManager::Instance()->addKeyListener(this, "FirstPersonCamera");
	setEnabled(enabled_);
}

void FirstPersonMovement::update()
{
	float deltaTime = RenderManager::Instance()->time()->deltaTime();
	setMovementAcceleration(deltaTime);

	if (velocity_ != Ogre::Vector3::ZERO)
	{
		gameObject()->transform()->translate(velocity_ * deltaTime);
	}
}

//Custom methods for this component
bool FirstPersonMovement::keyPressed(const OIS::KeyEvent & e)
{
	if (e.key == moveForwardsKey_) goingForward_ = true;
	else if (e.key == moveBackwardsKey_) goingBack_ = true;
	else if (e.key == moveLeftKey_) goingLeft_ = true;
	else if (e.key == moveRightKey_) goingRight_ = true;

	return true;
}

bool FirstPersonMovement::keyReleased(const OIS::KeyEvent & e)
{
	if (e.key == moveForwardsKey_) goingForward_ = false;
	else if (e.key == moveBackwardsKey_) goingBack_ = false;
	else if (e.key == moveLeftKey_) goingLeft_ = false;
	else if (e.key == moveRightKey_) goingRight_ = false;

	return true;
}

void FirstPersonMovement::setMovementAcceleration(float deltaTime)
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

	setMovementVelocity(topSpeed);
}

void FirstPersonMovement::setMovementVelocity(Ogre::Real topSpeed)
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