#include "GUIManager.h"

GUIManager* GUIManager::instance_ = nullptr;

GUIManager * GUIManager::Instance()
{
	if (instance_ == nullptr) instance_ = new GUIManager();
	return instance_;
}

GUIManager::GUIManager() :
	renderer_(nullptr),
	currentRoot_(nullptr)
{
}

void GUIManager::init()
{
	// Init CEGUI System
	renderer_ = &CEGUI::OgreRenderer::bootstrapSystem(*static_cast<Ogre::RenderTarget*>(RenderManager::Instance()->getWindow()));

	// Load CEGUI Resources
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	
	InputManager::Instance()->addKeyListener(this, "GUI");
	InputManager::Instance()->addMouseListener(this, "GUI");
}

void GUIManager::update(float deltaTime)
{
	CEGUI::System::getSingleton().injectTimePulse(deltaTime);
}

void GUIManager::close()
{
	CEGUI::OgreRenderer::destroySystem();
}



CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	default:
		return CEGUI::LeftButton;
	}
}

bool GUIManager::keyPressed(const OIS::KeyEvent & e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)e.key);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar((CEGUI::Key::Scan)e.text);
	return true;
}

bool GUIManager::keyReleased(const OIS::KeyEvent & e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)e.key);
	return true;
}

bool GUIManager::mouseMoved(const OIS::MouseEvent & e)
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectMouseMove(e.state.X.rel, e.state.Y.rel);
	if (e.state.Z.rel) context.injectMouseWheelChange(e.state.Z.rel / 120.0f);
	return true;
}

bool GUIManager::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	return true;
}

bool GUIManager::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}



CEGUI::Window* GUIManager::createWidget(const CEGUI::String & widgetType, const CEGUI::String & widgetName, CEGUI::Window * parent)
{
	CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* widget = windowManager.createWindow(widgetType, widgetName);
	if (parent) parent->addChild(widget);
	else currentRoot_->addChild(widget);
	return widget;
}

CEGUI::Window * GUIManager::createRootWidget(const CEGUI::String& rootName)
{
	CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* root = windowManager.createWindow("DefaultWindow", rootName);
	if (!currentRoot_) setRootWidget(root);
	return root;
}

void GUIManager::setRootWidget(CEGUI::Window * root)
{
	currentRoot_ = root;
	getContext().setRootWindow(root);
}