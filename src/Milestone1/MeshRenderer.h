#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_

#include "Component.h"
#include "OgreSystem.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(GameObject* container, const std::string& meshName, bool enabled = true);
	virtual ~MeshRenderer();

	virtual std::string getName() const { return name_; }

	inline void setMaterialName(const std::string& name) { entity_->setMaterialName(name); }

private:
	static std::string name_;
	Ogre::Entity* entity_;
};

#endif // !_MESH_RENDERER_H_