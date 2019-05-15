#include "GunController.h"

#include "RigidBody.h"
#include "AudioSource.h"
#include "FirstPersonCamera.h"

std::string GunController::name_ = "GunController";

GunController::GunController() :
	isFiring_(false),
	fireButton_(OIS::MouseButtonID::MB_Left),
	gunChangeButton_(OIS::KeyCode::KC_C)
{
}

GunController::~GunController()
{
}

void GunController::update()
{
	if (isFiring_)
	{
		static_cast<AudioSource*>(EntityComponentManager::Instance()->getComponent(gameObject(), "AudioSource"))->play();
		isFiring_ = false;
	}
}

void GunController::load(const std::map<std::string, ValueType>& params)
{
 	auto it = params.begin();
	it = params.find("enabled_gc");
	if (it != params.end()) enabled_ = params.at("enabled_gc").b;

	int i = 0;
	std::stringstream s;
	s << "mesh_name" << i;
	it = params.find(s.str());
	while (it != params.end()) {
		listGunsMeshes_.push_back((*it).second.s);
		i++;
		std::stringstream s1;
		s1 << "mesh_name" << i;
		it = params.find(s1.str());
	}
}

Component * GunController::clone()
{
	GunController* clonedComponent = new GunController();

	clonedComponent->enabled_ = this->enabled_;
	clonedComponent->fireButton_ = this->fireButton_;
	clonedComponent->gunChangeButton_ = this->gunChangeButton_;
	clonedComponent->listGunsMeshes_ = this->listGunsMeshes_;


	return clonedComponent;
}

void GunController::init()
{
	InputManager::Instance()->addMouseListener(this, "GunController");
	InputManager::Instance()->addKeyListener(this, "GunController");
	setEnabled(enabled_);
}

bool GunController::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	if (id == OIS::MouseButtonID::MB_Left)
	{
		isFiring_ = true;

		GameObject* player = EntityComponentManager::Instance()->findGameObjectWithTag("Player");
		Ogre::Camera* cam = static_cast<FirstPersonCamera*>(EntityComponentManager::Instance()->getComponent(player, "FirstPersonCamera"))->getCamera();

		Ogre::Vector3 f = cam->getRealPosition();
		Ogre::Vector3 n = cam->getRealDirection();
		Ogre::Quaternion q = cam->getRealOrientation();
		f = f + n * 10;
		btVector3 from(f.x, f.y, f.z);
		btVector3 normal(n.x, n.y, n.z);

		GameObject* bullet = EntityComponentManager::Instance()->instantiate("Bullet", f, q);

		RigidBody* rb = static_cast<RigidBody*>(EntityComponentManager::Instance()->getComponent(bullet, "RigidBody"));
		rb->rigidbody()->applyCentralImpulse(normal * 1000.0f);

		//RaycastHit hit;
		//if (Physics::Instance()->raycast(from, normal, 1000.0f, &hit))
		//{
		//	std::cout << hit.gameObject->getName() << std::endl;
		//}
	}

	return true;
}

bool GunController::keyPressed(const OIS::KeyEvent &e) {
	if (e.key == gunChangeButton_)
	{
		if (listGunsMeshes_.size() > 1) {
			std::string gunActive = listGunsMeshes_.front();
			listGunsMeshes_.pop_front();
			static_cast<MeshRenderer*>(EntityComponentManager::Instance()->
				getComponent(this->container_, "MeshRenderer"))->changeMesh(listGunsMeshes_.front(), "");
			listGunsMeshes_.push_back(gunActive);
			std::cout << "Gun changed: " << listGunsMeshes_.back() << " to " << listGunsMeshes_.front();
		}
	}
	return true;
}

bool GunController::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	return true;
}