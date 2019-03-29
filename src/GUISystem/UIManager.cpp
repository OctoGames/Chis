#include "UIManager.h"

UIManager* UIManager::instance_ = nullptr;

UIManager * UIManager::Instance()
{
	if (instance_ == nullptr) instance_ = new UIManager();
	return instance_;
}

UIManager::UIManager() :
	renderer_(nullptr)
{
	InputManager::Instance()->addKeyListener(this, "GUI");
	InputManager::Instance()->addMouseListener(this, "GUI");
}

void UIManager::init()
{
	// Init CEGUI System
	renderer_ = &CEGUI::OgreRenderer::bootstrapSystem(*static_cast<Ogre::RenderTarget*>(RenderManager::Instance()->getWindow()));

	// Load CEGUI Resources
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	// Set default resources
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	// Create CEGUI root object
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	quit->setText("Quit");
	sheet->addChild(quit);

	quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&UIManager::quit, this));
}

void UIManager::close()
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

bool UIManager::keyPressed(const OIS::KeyEvent & e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)e.key);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar((CEGUI::Key::Scan)e.text);
	return true;
}

bool UIManager::keyReleased(const OIS::KeyEvent & e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)e.key);
	return true;
}

bool UIManager::mouseMoved(const OIS::MouseEvent & e)
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectMouseMove(e.state.X.rel, e.state.Y.rel);
	if (e.state.Z.rel) context.injectMouseWheelChange(e.state.Z.rel / 120.0f);
	return true;
}

bool UIManager::mousePressed(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	return true;
}

bool UIManager::mouseReleased(const OIS::MouseEvent & e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

bool UIManager::frameRenderingQueued(const Ogre::FrameEvent & evt)
{
	if (RenderManager::Instance()->getWindow()->isClosed()) return false;
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	return true;
}

void UIManager::quit()
{
	RenderManager::Instance()->setRunning(false);
}