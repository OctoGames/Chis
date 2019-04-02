#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include "Component.h"
#include "RenderManager.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	MeshRenderer(GameObject* container, const std::string& meshName, bool enabled = true);
	virtual ~MeshRenderer();

	virtual void init(const std::map<std::string, ValueType>& params);
	virtual std::string getName() const { return name_; }

	inline void setMaterialName(const std::string& name) { entity_->setMaterialName(name); }

private:
	static std::string name_;
	Ogre::Entity* entity_;
};

class MeshRendererFactory : public BaseFactory
{
public:
	MeshRendererFactory() {}
	virtual ~MeshRendererFactory() {}

	virtual Component* create() { return new MeshRenderer(); }
};

#endif // !__MESH_RENDERER_H__