#include "FirstPersonCamera.h"

#include "Transform.h"
#include "EntityComponentManager.h"

std::string FirstPersonCamera::name_ = "FirstPersonCamera";

FirstPersonCamera::FirstPersonCamera() :
	maxSpeed_(0),
	pitchLimit_(0.0f),
	velocity_(Ogre::Vector3::ZERO),
	goingForward_(false),
	goingBack_(false),
	goingLeft_(false),
	goingRight_(false),
	fastMove_(false),
	changeBackground_(false),
	camera_(nullptr),
	viewport_(nullptr),
	cameraNode_(nullptr)
{
}

FirstPersonCamera::FirstPersonCamera(GameObject* container, bool enabled) : 
	Component(container, enabled),
	maxSpeed_(200),
	pitchLimit_(180.0f),
	velocity_(Ogre::Vector3::ZERO), 
	goingForward_(false), 
	goingBack_(false), 
	goingLeft_(false), 
	goingRight_(false), 
	fastMove_(false),
	changeBackground_(false),
	camera_(nullptr),
	viewport_(nullptr),
	cameraNode_(nullptr)
{
	camera_ = RenderManager::Instance()->getSceneManager()->createCamera("FirstPersonCamera");
	cameraNode_ = static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->getNode();
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(camera_);
	cameraNode_->setPosition(0, 0, 500);
	cameraNode_->setFixedYawAxis(true);

	viewport_ = RenderManager::Instance()->getWindow()->addViewport(camera_);
	viewport_->setBackgroundColour(Ogre::ColourValue(1, 0, 1));
	viewport_->setAutoUpdated(true);

	float ratio = float(viewport_->getActualWidth()) / float(viewport_->getActualHeight());
	camera_->setFarClipDistance(10000.0f);
	camera_->setNearClipDistance(1.5f);
	camera_->setAspectRatio(ratio);

	InputManager::Instance()->addKeyListener(this, "FirstPersonCamera");
	InputManager::Instance()->addMouseListener(this, "FirstPersonCamera");
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::init(const std::map<std::string, ValueType>& params)
{
	camera_ = RenderManager::Instance()->getSceneManager()->createCamera("FirstPersonCamera");
	cameraNode_ = static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->getNode();
	static_cast<Transform*>(EntityComponentManager::Instance()->getComponent(gameObject(), "Transform"))->attachEntity(camera_);
	cameraNode_->setPosition(params.at("position_x").f, params.at("position_y").f, params.at("position_z").f);
	cameraNode_->setFixedYawAxis(params.at("yaw_fixed").b);

	viewport_ = RenderManager::Instance()->getWindow()->addViewport(camera_);
	viewport_->setBackgroundColour(Ogre::ColourValue(params.at("color_r").f, params.at("color_g").f, params.at("color_b").f));
	viewport_->setAutoUpdated(true);

	bool infiniteClip = RenderManager::Instance()->getRoot()->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE);
	if (infiniteClip) camera_->setFarClipDistance(0);
	else camera_->setFarClipDistance(params.at("far_clip").f);
	camera_->setNearClipDistance(params.at("near_clip").f);
	float ratio = float(viewport_->getActualWidth()) / float(viewport_->getActualHeight());
	camera_->setAspectRatio(ratio);

	InputManager::Instance()->addKeyListener(this, "FirstPersonCamera");
	InputManager::Instance()->addMouseListener(this, "FirstPersonCamera");

	setEnabled(params.at("enabled").b);
}

void FirstPersonCamera::update()
{
	float deltaTime = RenderManager::Instance()->time()->deltaTime();
	setCameraAcceleration(deltaTime);	

	if (velocity_ != Ogre::Vector3::ZERO)
	{
		cameraNode_->translate(velocity_ * deltaTime);
	}

	if (changeBackground_)
	{
		float red = Ogre::Math::RangeRandom(0.1f, 0.9f);
		float green = Ogre::Math::RangeRandom(0.1f, 0.9f);
		float blue = Ogre::Math::RangeRandom(0.1f, 0.9f);
		setBackgroundColour(Ogre::ColourValue(red, green, blue));
		changeBackground_ = false;
	}
}

bool FirstPersonCamera::keyPressed(const OIS::KeyEvent & e)
{
	switch (e.key)
	{
	case OIS::KC_W:
		goingForward_ = true;
		break;

	case OIS::KC_S:
		goingBack_ = true;
		break;

	case OIS::KC_A:
		goingLeft_ = true;
		break;

	case OIS::KC_D:
		goingRight_ = true;
		break;

	case OIS::KC_LSHIFT:
		fastMove_ = !fastMove_;
		break;
	}

	return true;
}

bool FirstPersonCamera::keyReleased(const OIS::KeyEvent & e)
{
	switch (e.key)
	{
	case OIS::KC_W:
		goingForward_ = false;
		break;

	case OIS::KC_S:
		goingBack_ = false;
		break;

	case OIS::KC_A:
		goingLeft_ = false;
		break;

	case OIS::KC_D:
		goingRight_ = false;
		break;
	}

	return true;
}

bool FirstPersonCamera::mouseMoved(const OIS::MouseEvent & e)
{
	float relY = -e.state.Y.rel;
	float relX = -e.state.X.rel;
	float currentCameraPitch = cameraNode_->getOrientation().getPitch().valueDegrees();
	float nextCameraPitch = currentCameraPitch + (relY * 0.15f);

	if (nextCameraPitch < pitchLimit_ && nextCameraPitch > -pitchLimit_) cameraNode_->pitch(Ogre::Degree(relY * 0.15f));
	cameraNode_->yaw(Ogre::Degree(relX * 0.15f), Ogre::Node::TS_PARENT);

	return true;
}

bool FirstPersonCamera::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	switch (id)
	{
	case OIS::MB_Left:
		changeBackground_ = true;
		break;
	}
	return true;
}

bool FirstPersonCamera::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	return true;
}

void FirstPersonCamera::setCameraAcceleration(float deltaTime)
{
	Ogre::Vector3 accel = Ogre::Vector3::ZERO;
	Ogre::Matrix3 axes = cameraNode_->getLocalAxes();
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
