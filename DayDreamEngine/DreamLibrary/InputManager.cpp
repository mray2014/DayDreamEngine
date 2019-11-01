#include "InputManager.h"
#include "KeyCode.h"
#include <vector>

InputManager* InputManager::instance = 0;
static std::unordered_map<int, Binding> Actions;


InputManager * InputManager::GetInstance()
{
	if (instance == 0)
	{
		instance = new InputManager();
	}
	return instance;
}

void InputManager::Release()
{
	if (instance != 0) { delete instance; instance = 0; };
}
void InputManager::Update()
{
	DreamInput::Update();
}
bool InputManager::IsControllerConnected()
{
	return DreamInput::IsControllerConnected();
}
float InputManager::GetLeftStickX()
{
	return DreamInput::GetLStickX();
}
float InputManager::GetLeftStickY()
{
	return DreamInput::GetLStickY();
}
float InputManager::GetRightStickX()
{
	return DreamInput::GetRStickX();
}
float InputManager::GetRightStickY()
{
	return DreamInput::GetRStickY();
}
bool InputManager::IsKeyUp(int key)
{
	bool keyboardUp = DreamInput::KeyUp(key);

	if (keyboardUp)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsKeyDown(int key)
{
	bool keyboardDown = DreamInput::KeyDown(key);


	if (keyboardDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsKeyPressed(int key)
{
	bool keyboardPressed= DreamInput::KeyPressed(key);

	if (keyboardPressed)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsKeyReleased(int key)
{
	bool keyboardReleased = DreamInput::KeyReleased(key);

	if (keyboardReleased)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsButtonUp(int key)
{
	bool controllerUp = DreamInput::ButtonUp(key);

	if (controllerUp)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsButtonDown(int key)
{
	bool controllerDown = DreamInput::ButtonDown(key);

	if (controllerDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsButtonPressed(int key)
{
	bool controllerPressed = DreamInput::ButtonPressed(key);

	if (controllerPressed)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsButtonReleased(int key)
{
	bool controllerReleased = DreamInput::ButtonReleased(key);

	if (controllerReleased)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}
bool InputManager::IsActionUp(int act)
{
	Binding newBind = Actions[act];

	bool keyboardUp = DreamInput::KeyUp(newBind.keyboard);
	bool controllerUp = DreamInput::ButtonUp(newBind.controller);

	if (keyboardUp || controllerUp) 
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool InputManager::IsActionDown(int act)
{
	Binding newBind = Actions[act];

	bool keyboardDown = DreamInput::KeyDown(newBind.keyboard);
	bool controllerDown = DreamInput::ButtonDown(newBind.controller);

	if (keyboardDown || controllerDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsActionPressed(int act)
{
	Binding newBind = Actions[act];

	bool keyboardPressed = DreamInput::KeyPressed(newBind.keyboard);
	bool controllerPressed = DreamInput::ButtonPressed(newBind.controller);

	if (keyboardPressed || controllerPressed)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsActionReleased(int act)
{
	Binding newBind = Actions[act];

	bool keyboardReleased = DreamInput::KeyReleased(newBind.keyboard);
	bool controllerReleased = DreamInput::ButtonReleased(newBind.controller);

	if (keyboardReleased || controllerReleased)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void InputManager::AddActionBinding(int act, Binding bind)
{
	Actions[act] = bind;
}
void InputManager::AddActionBinding(int act, int keyboardBind, int controllerBind)
{
	Actions[act] = { keyboardBind, controllerBind };
}
void InputManager::ChangeActionBinding(int act, Binding bind)
{
	Actions[act] = bind;
}
void InputManager::ChangeActionBinding(int act, int keyboardBind, int controllerBind)
{
	Actions[act] = { keyboardBind, controllerBind };
}
void InputManager::ChangeActionKeyBoardBinding(int act, int keyboardBind)
{
	Actions[act].keyboard = keyboardBind;
}
void InputManager::ChangeActionControllerBinding(int act, int controllerBind)
{
	Actions[act].controller = controllerBind;
}
InputManager::InputManager() {
	DreamInput::Initialize();
}
