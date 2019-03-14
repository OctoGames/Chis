#include "debugDrawer.h"

debugDrawer* debugDrawer::instance_ = nullptr;

debugDrawer::debugDrawer()
{

}

debugDrawer* debugDrawer::Instance()
{
	if (instance_ == nullptr)
	{
		instance_ = new debugDrawer();
	}
	return instance_;
}

void debugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	Ogre::Vector3 coords[2];

	coords[0].x = from.getX();
	coords[0].y = from.getY();
	coords[0].z = from.getZ();

	coords[1].x = to.getX();
	coords[1].y = to.getY();
	coords[1].z = to.getZ();

	Ogre::ManualObject* line = OgreSystem::Instance()->getSceneManager()->createManualObject("pepito");

	line->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);

	line->position(coords[0]);
	line->position(coords[1]);

	line->end();

	OgreSystem::Instance()->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject(line);
}