#include "InputManager.h"

#include "RenderManager.h"

InputManager* InputManager::instance_ = nullptr;

InputManager* InputManager::Instance()
{
	if (instance_ == nullptr) instance_ = new InputManager();
	return instance_;
}

InputManager::InputManager() :
	mouse_(nullptr),
	keyboard_(nullptr),
	inputManager_(nullptr)
{
}

void InputManager::init()
{
	if (!inputManager_) 
	{
		// Setup basic variables
		OIS::ParamList paramList;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		// Get window handle
		RenderManager::Instance()->getWindow()->getCustomAttribute("WINDOW", &windowHnd);

		// Fill parameter list
		windowHndStr << (unsigned int)windowHnd;
		paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		//paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
		//paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));

		// Create inputsystem
		inputManager_ = OIS::InputManager::createInputSystem(paramList);

		// If possible create a buffered keyboard
		if (inputManager_->getNumberOfDevices(OIS::OISKeyboard) > 0) 
		{
			keyboard_ = static_cast<OIS::Keyboard*>(inputManager_->createInputObject(OIS::OISKeyboard, true));
			keyboard_->setEventCallback(this);
		}

		// If possible create a buffered mouse
		if (inputManager_->getNumberOfDevices(OIS::OISMouse) > 0) 
		{
			mouse_ = static_cast<OIS::Mouse*>(inputManager_->createInputObject(OIS::OISMouse, true));
			mouse_->setEventCallback(this);

			// Get window size
			int left, top;
			unsigned int width, height, depth;
			RenderManager::Instance()->getWindow()->getMetrics(width, height, depth, left, top);

			// Set mouse region
			setWindowExtents(width, height);
		}

		// If possible create all joysticks in buffered mode
		if (inputManager_->getNumberOfDevices(OIS::OISJoyStick) > 0) 
		{
			joysticks_.resize(inputManager_->getNumberOfDevices(OIS::OISJoyStick));

			auto itJoystick = joysticks_.begin();
			auto itJoystickEnd = joysticks_.end();
			for (; itJoystick != itJoystickEnd; ++itJoystick) 
			{
				(*itJoystick) = static_cast<OIS::JoyStick*>(inputManager_->createInputObject(OIS::OISJoyStick, true));
				(*itJoystick)->setEventCallback(this);
			}
		}
	}
}

void InputManager::update(float deltaTime)
{
	if (mouse_) mouse_->capture();
	if (keyboard_) keyboard_->capture();
	if (joysticks_.size() > 0) for (OIS::JoyStick* joystick : joysticks_) joystick->capture();
}

void InputManager::close()
{
	if (inputManager_) 
	{
		if (mouse_) 
		{
			inputManager_->destroyInputObject(mouse_);
			mouse_ = nullptr;
		}

		if (keyboard_)
		{
			inputManager_->destroyInputObject(keyboard_);
			keyboard_ = nullptr;
		}

		if (joysticks_.size() > 0) 
		{
			for (OIS::JoyStick* joystick : joysticks_) 
			{
				inputManager_->destroyInputObject(joystick);
				joystick = nullptr;
			}

			joysticks_.clear();
		}

		inputManager_->destroyInputSystem(inputManager_);
		inputManager_ = nullptr;

		removeAllListeners();
	}
}

void InputManager::addKeyListener(OIS::KeyListener * keyListener, const std::string & instanceName)
{
	keyListeners_[instanceName] = keyListener;
}

void InputManager::addMouseListener(OIS::MouseListener * mouseListener, const std::string & instanceName)
{
	mouseListeners_[instanceName] = mouseListener;
}

void InputManager::addJoystickListener(OIS::JoyStickListener * joystickListener, const std::string & instanceName)
{
	joystickListeners_[instanceName] = joystickListener;
}

void InputManager::removeKeyListener(const std::string & instanceName)
{
	auto itKeyListener = keyListeners_.find(instanceName);
	if (itKeyListener != keyListeners_.end()) keyListeners_.erase(itKeyListener);
}

void InputManager::removeMouseListener(const std::string & instanceName)
{
	auto itMouseListener = mouseListeners_.find(instanceName);
	if (itMouseListener != mouseListeners_.end()) mouseListeners_.erase(itMouseListener);
}

void InputManager::removeJoystickListener(const std::string & instanceName)
{
	auto itJoystickListener = joystickListeners_.find(instanceName);
	if (itJoystickListener != joystickListeners_.end()) joystickListeners_.erase(itJoystickListener);
}

void InputManager::removeKeyListener(OIS::KeyListener * keyListener) 
{
	auto itKeyListener = keyListeners_.begin();
	auto itKeyListenerEnd = keyListeners_.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) 
	{
		if (itKeyListener->second == keyListener) 
		{
			keyListeners_.erase(itKeyListener);
			break;
		}
	}
}

void InputManager::removeMouseListener(OIS::MouseListener * mouseListener) 
{
	auto itMouseListener = mouseListeners_.begin();
	auto itMouseListenerEnd = mouseListeners_.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) 
	{
		if (itMouseListener->second == mouseListener) 
		{
			mouseListeners_.erase(itMouseListener);
			break;
		}
	}
}

void InputManager::removeJoystickListener(OIS::JoyStickListener * joystickListener) 
{
	auto itJoystickListener = joystickListeners_.begin();
	auto itJoystickListenerEnd = joystickListeners_.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener) 
	{
		if (itJoystickListener->second == joystickListener) 
		{
			joystickListeners_.erase(itJoystickListener);
			break;
		}
	}
}

void InputManager::removeAllListeners() 
{
	removeAllKeyListeners();
	removeAllMouseListeners();
	removeAllJoystickListeners();
}

void InputManager::removeAllKeyListeners() 
{
	keyListeners_.clear();
}

void InputManager::removeAllMouseListeners() 
{
	mouseListeners_.clear();
}

void InputManager::removeAllJoystickListeners() 
{
	joystickListeners_.clear();
}

void InputManager::setWindowExtents(int width, int height)
{
	const OIS::MouseState &mouseState = mouse_->getMouseState();
	mouseState.width = width;
	mouseState.height = height;
}

bool InputManager::keyPressed(const OIS::KeyEvent &e) 
{
	auto itKeyListener = keyListeners_.begin();
	auto itKeyListenerEnd = keyListeners_.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) 
	{
		if (!itKeyListener->second->keyPressed(e)) break;
	}

	return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &e) 
{
	auto itKeyListener = keyListeners_.begin();
	auto itKeyListenerEnd = keyListeners_.end();
	for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) 
	{
		if (!itKeyListener->second->keyReleased(e)) break;
	}

	return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent &e) 
{
	auto itMouseListener = mouseListeners_.begin();
	auto itMouseListenerEnd = mouseListeners_.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener) 
	{
		if (!itMouseListener->second->mouseMoved(e)) break;
	}

	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) 
{
	auto itMouseListener = mouseListeners_.begin();
	auto itMouseListenerEnd = mouseListeners_.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener)
	{
		if (!itMouseListener->second->mousePressed(e, id)) break;
	}

	return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) 
{
	auto itMouseListener = mouseListeners_.begin();
	auto itMouseListenerEnd = mouseListeners_.end();
	for (; itMouseListener != itMouseListenerEnd; ++itMouseListener)
	{
		if (!itMouseListener->second->mouseReleased(e, id)) break;
	}

	return true;
}

bool InputManager::povMoved(const OIS::JoyStickEvent &e, int pov) 
{
	auto itJoystickListener = joystickListeners_.begin();
	auto itJoystickListenerEnd = joystickListeners_.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
	{
		if (!itJoystickListener->second->povMoved(e, pov)) break;
	}

	return true;
}

bool InputManager::axisMoved(const OIS::JoyStickEvent &e, int axis) 
{
	auto itJoystickListener = joystickListeners_.begin();
	auto itJoystickListenerEnd = joystickListeners_.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener) 
	{
		if (!itJoystickListener->second->axisMoved(e, axis)) break;
	}

	return true;
}

bool InputManager::sliderMoved(const OIS::JoyStickEvent &e, int sliderID) 
{
	auto itJoystickListener = joystickListeners_.begin();
	auto itJoystickListenerEnd = joystickListeners_.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
	{
		if (!itJoystickListener->second->sliderMoved(e, sliderID)) break;
	}

	return true;
}

bool InputManager::buttonPressed(const OIS::JoyStickEvent &e, int button)
{
	auto itJoystickListener = joystickListeners_.begin();
	auto itJoystickListenerEnd = joystickListeners_.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
	{
		if (!itJoystickListener->second->buttonPressed(e, button)) break;
	}

	return true;
}

bool InputManager::buttonReleased(const OIS::JoyStickEvent &e, int button) 
{
	auto itJoystickListener = joystickListeners_.begin();
	auto itJoystickListenerEnd = joystickListeners_.end();
	for (; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
	{
		if (!itJoystickListener->second->buttonReleased(e, button)) break;
	}

	return true;
}