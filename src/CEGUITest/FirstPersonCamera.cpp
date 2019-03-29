#include "FirstPersonCamera.h"
#include "InputManager.h"

FirstPersonCamera::FirstPersonCamera() : 
	maxSpeed_(200),
	pitchLimit_(10.0f),
	velocity_(Ogre::Vector3::ZERO), 
	goingForward_(false), 
	goingBack_(false), 
	goingLeft_(false), 
	goingRight_(false), 
	fastMove_(false),
	changeBackground_(false),
	light_(nullptr),
	camera_(nullptr),
	viewport_(nullptr),
	cameraNode_(nullptr)
{
	camera_ = RenderManager::Instance()->getSceneManager()->createCamera("FirstPersonCamera");
	cameraNode_ = RenderManager::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode("CameraNode");
	cameraNode_->setPosition(0, 0, 500);
	cameraNode_->attachObject(camera_);
	cameraNode_->setFixedYawAxis(true);

	viewport_ = RenderManager::Instance()->getWindow()->addViewport(camera_);
	viewport_->setBackgroundColour(Ogre::ColourValue(1, 0, 1));
	viewport_->setAutoUpdated(true);

	float ratio = float(viewport_->getActualWidth()) / float(viewport_->getActualHeight());
	camera_->setFarClipDistance(10000.0f);
	camera_->setNearClipDistance(1.5f);
	camera_->setAspectRatio(ratio);

	light_ = RenderManager::Instance()->getSceneManager()->createLight("CameraLight");
	light_->setType(Ogre::Light::LT_DIRECTIONAL);
	light_->setDiffuseColour(1, 1, 1);
	cameraNode_->attachObject(light_);

	InputManager::Instance()->addKeyListener(this, "FirstPersonCamera");
	InputManager::Instance()->addMouseListener(this, "FirstPersonCamera");
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::update(float deltaTime)
{
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
