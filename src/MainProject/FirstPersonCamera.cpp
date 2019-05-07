#include "FirstPersonCamera.h"

#include "EntityComponentManager.h"
#include "GUIManager.h"
#include "Physics.h"

std::string FirstPersonCamera::name_ = "FirstPersonCamera";

FirstPersonCamera::FirstPersonCamera() :
	camera_(nullptr),
	viewport_(nullptr),
	farClipDistance_(0.0f),	
	nearClipDistance_(0.0f),	
	backgroundColour_(Ogre::ColourValue::ZERO),	
	maxSpeed_(0.0f),	
	pitchLimit_(0.0f),
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

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::load(const std::map<std::string, ValueType>& params)
{
	enabled_ = params.at("enabled_fpc").b;
	farClipDistance_ = params.at("far_clip").f;
	nearClipDistance_ = params.at("near_clip").f;
	backgroundColour_ = Ogre::ColourValue(params.at("color_r").f, params.at("color_g").f, params.at("color_b").f);

	maxSpeed_ = params.at("max_speed").f;
	pitchLimit_ = params.at("pitch_limit").f;

	//moveForwardsKey_ = static_cast<OIS::KeyCode>(params.at("forward_key").i);
	//moveBackwardsKey_ = static_cast<OIS::KeyCode>(params.at("backward_key").i);
	//moveLeftKey_ = static_cast<OIS::KeyCode>(params.at("left_key").i);
	//moveRightKey_ = static_cast<OIS::KeyCode>(params.at("right_key").i);
	//fastMoveKey_ = static_cast<OIS::KeyCode>(params.at("fast_key").i);
}

Component * FirstPersonCamera::clone()
{
	FirstPersonCamera* clonedComponent = new FirstPersonCamera();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->farClipDistance_ = this->farClipDistance_;
	clonedComponent->nearClipDistance_ = this->nearClipDistance_;
	clonedComponent->backgroundColour_ = this->backgroundColour_;

	clonedComponent->maxSpeed_ = this->maxSpeed_;
	clonedComponent->pitchLimit_ = this->pitchLimit_;

	clonedComponent->moveForwardsKey_ = this->moveForwardsKey_;
	clonedComponent->moveBackwardsKey_ = this->moveBackwardsKey_;
	clonedComponent->moveLeftKey_ = this->moveLeftKey_;
	clonedComponent->moveRightKey_ = this->moveRightKey_;
	clonedComponent->fastMoveKey_ = this->fastMoveKey_;

	return clonedComponent;
}

void FirstPersonCamera::init()
{
	camera_ = RenderManager::Instance()->getSceneManager()->createCamera("FirstPersonCamera_" + gameObject()->getName());
	gameObject()->transform()->attachObject(camera_);
	gameObject()->transform()->setFixedYawAxis(true);

	viewport_ = RenderManager::Instance()->getWindow()->addViewport(camera_);
	viewport_->setBackgroundColour(backgroundColour_);
	viewport_->setAutoUpdated(true);

	camera_->setAspectRatio(float(viewport_->getActualWidth()) / float(viewport_->getActualHeight()));
	bool infiniteClip = RenderManager::Instance()->getRoot()->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE);
	if (infiniteClip) camera_->setFarClipDistance(0.0f);
	else camera_->setFarClipDistance(farClipDistance_);
	camera_->setNearClipDistance(nearClipDistance_);

	InputManager::Instance()->addKeyListener(this, "FirstPersonCamera");
	InputManager::Instance()->addMouseListener(this, "FirstPersonCamera");

	setEnabled(enabled_);
}

void FirstPersonCamera::update()
{
	float deltaTime = RenderManager::Instance()->time()->deltaTime();
	setCameraAcceleration(deltaTime);	

	if (velocity_ != Ogre::Vector3::ZERO)
	{
		gameObject()->transform()->translate(velocity_ * deltaTime);
	}
}

bool FirstPersonCamera::keyPressed(const OIS::KeyEvent & e)
{
	if (e.key == moveForwardsKey_) goingForward_ = true;
	else if (e.key == moveBackwardsKey_) goingBack_ = true;
	else if (e.key == moveLeftKey_) goingLeft_ = true;
	else if (e.key == moveRightKey_) goingRight_ = true;

	return true;
}

bool FirstPersonCamera::keyReleased(const OIS::KeyEvent & e)
{
	if (e.key == moveForwardsKey_) goingForward_ = false;
	else if (e.key == moveBackwardsKey_) goingBack_ = false;
	else if (e.key == moveLeftKey_) goingLeft_ = false;
	else if (e.key == moveRightKey_) goingRight_ = false;

	return true;
}

bool FirstPersonCamera::mouseMoved(const OIS::MouseEvent & e)
{
	float relY = -e.state.Y.rel;
	float relX = -e.state.X.rel;
	float currentCameraPitch = gameObject()->transform()->getOrientation().getPitch().valueDegrees();
	float nextCameraPitch = currentCameraPitch + (relY * 0.15f);

	if (nextCameraPitch < pitchLimit_ && nextCameraPitch > -pitchLimit_) gameObject()->transform()->pitch(Ogre::Degree(relY * 0.15f));
	gameObject()->transform()->yaw(Ogre::Degree(relX * 0.15f), Ogre::Node::TS_PARENT);

	return true;
}

void FirstPersonCamera::setCameraAcceleration(float deltaTime)
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

	setCameraVelocity(topSpeed);
}

void FirstPersonCamera::setCameraVelocity(Ogre::Real topSpeed)
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
