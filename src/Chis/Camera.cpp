#include "Camera.h"


std::string Camera::name_ = "Camera";

Camera::Camera() :
	camera_(nullptr),
	viewport_(nullptr),
	farClipDistance_(0.0f),
	nearClipDistance_(0.0f),
	backgroundColour_(Ogre::ColourValue::ZERO)
{
}

Camera::~Camera()
{
	if (camera_)
	{
		RenderManager::Instance()->getSceneManager()->destroyCamera(camera_);
		camera_ = nullptr;
	}
}

void Camera::load(const std::map<std::string, ValueType>& params)
{
	enabled_ = params.at("enabled_c").b;
	farClipDistance_ = params.at("far_clip").f;
	nearClipDistance_ = params.at("near_clip").f;
	backgroundColour_ = Ogre::ColourValue(params.at("color_r").f, params.at("color_g").f, params.at("color_b").f);
}

Component * Camera::clone()
{
	Camera* clonedComponent = new Camera();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->farClipDistance_ = this->farClipDistance_;
	clonedComponent->nearClipDistance_ = this->nearClipDistance_;
	clonedComponent->backgroundColour_ = this->backgroundColour_;
	
	return clonedComponent;
}

void Camera::init()
{
	camera_ = RenderManager::Instance()->getSceneManager()->createCamera("MenuCamera_" + gameObject()->getName());
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

	setEnabled(enabled_);
}
