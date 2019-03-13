#pragma once

#include "component.h"
#include "OgreSystem.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(GameObject* container, const std::string& meshName, bool enabled = true);
	virtual ~MeshRenderer();

	virtual std::string getName() const { return name_; }

	// maybe there will be a list of materials bc they can be more than one
	inline void setMaterialName(const std::string& name) { entity_->setMaterialName(name); }

private:
	static std::string name_;
	Ogre::Entity* entity_;
};