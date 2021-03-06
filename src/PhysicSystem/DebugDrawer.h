#ifndef __DEBUG_DRAWER_H__
#define __DEBUG_DRAWER_H__

#include <btBulletDynamicsCommon.h>

//Shows the collider of the object
class DebugDrawer : public btIDebugDraw 
{
public:
	static DebugDrawer& getSingleton()
	{
		static DebugDrawer instance_;
		return instance_;
	}
	static DebugDrawer* Instance();

	void init() {};
	void deinit() {};
	void debugPhysics(bool enable) {};

	virtual void drawTriangle(const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar) { (void)v0; (void)v1; (void)v2; (void)color; }
	virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) { (void)PointOnB; (void)normalOnB; (void)distance; (void)lifeTime; (void)color; }
	virtual void reportErrorWarning(const char *warningString) { (void)warningString; }
	virtual void draw3dText(const btVector3 &location, const char *textString) { (void)location; (void)textString; }
	virtual void setDebugMode(int debugMode) { debugModes_ = (DebugDrawModes)debugMode; }
	virtual int getDebugMode() const { return debugModes_; }

	void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
	void drawCube(btVector3 position, btVector3 scale);
	void resetLineNumber();

private:
	DebugDrawer();
	virtual ~DebugDrawer() {};

	static DebugDrawer* instance_;

	int numberOfLines_;
	bool isDebuggingPhysics_;
	DebugDrawModes debugModes_;
};

#endif // !__DEBUG_DRAWER_H__