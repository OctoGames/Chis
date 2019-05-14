#include "FirstPersonCamera.h"


std::string FirstPersonCamera::name_ = "FirstPersonCamera";

FirstPersonCamera::FirstPersonCamera() :
	camera_(nullptr),
	viewport_(nullptr),
	pitchLimit_(0.0f),
	farClipDistance_(0.0f),
	nearClipDistance_(0.0f),
	backgroundColour_(Ogre::ColourValue::ZERO)
{
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_fpc"); if (it != params.end()) enabled_ = params.at("enabled_fpc").b;
	it = params.find("far_clip"); if (it != params.end()) farClipDistance_ = params.at("far_clip").f;
	it = params.find("near_clip"); if (it != params.end()) nearClipDistance_ = params.at("near_clip").f;
	it = params.find("pitch_limit"); if (it != params.end()) pitchLimit_ = params.at("pitch_limit").f;
	it = params.find("color_r"); if (it != params.end())
	{
		it = params.find("color_g"); if (it != params.end())
		{
			it = params.find("color_b"); if (it != params.end())
			{
				backgroundColour_ = Ogre::ColourValue(params.at("color_r").f, params.at("color_g").f, params.at("color_b").f);
			}
		}
	}
}

Component * FirstPersonCamera::clone()
{
	FirstPersonCamera* clonedComponent = new FirstPersonCamera();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->farClipDistance_ = this->farClipDistance_;
	clonedComponent->nearClipDistance_ = this->nearClipDistance_;
	clonedComponent->backgroundColour_ = this->backgroundColour_;
	clonedComponent->pitchLimit_ = this->pitchLimit_;

	return clonedComponent;
}

void FirstPersonCamera::init()
{
	camera_ = RenderManager::Instance()->getSceneManager()->createCamera("FirstPersonCamera");
	gameObject()->transform()->attachObject(camera_);
	gameObject()->transform()->setFixedYawAxis(true);

	viewport_ = RenderManager::Instance()->getWindow()->addViewport(camera_, 1);
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


bool FirstPersonCamera::mouseMoved(const OIS::MouseEvent & e)
{
	float relY = -e.state.Y.rel;
	float relX = -e.state.X.rel;
	float currentCameraPitch = gameObject()->transform()->getOrientation().getPitch().valueDegrees();
	float nextCameraPitch = currentCameraPitch + (relY * 0.15f);

	if (nextCameraPitch < pitchLimit_ && nextCameraPitch > -pitchLimit_) gameObject()->transform()->pitch(Ogre::Degree(relY * 0.15f));
	gameObject()->transform()->yaw(Ogre::Degree(relX * 0.15f), Ogre::Node::TS_PARENT);

	RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(gameObject(), "RigidBody"));

	btTransform tr;
	Ogre::Vector3 position = gameObject()->transform()->getPosition();
	btVector3 pos(position.x, position.y, position.z);
	tr.setOrigin(pos);
	Ogre::Quaternion orientation = gameObject()->transform()->getOrientation();
	btQuaternion quat(orientation.x, orientation.y, orientation.z, orientation.w);
	tr.setRotation(quat);
	rb->rigidbody()->setCenterOfMassTransform(tr);

	

	return true;
}