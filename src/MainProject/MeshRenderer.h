#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include "Component.h"
#include "RenderManager.h"

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

#endif // !__MESH_RENDERER_H__