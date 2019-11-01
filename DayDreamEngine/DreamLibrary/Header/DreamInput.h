#pragma once
#include <vector>
//#include<unordered_map>

namespace DreamInput
{
	enum ControllerButton
	{
		BUTTON_L3,
		BUTTON_R3,
		BUTTON_OPTIONS,
		DPAD_UP,
		DPAD_RIGHT,
		DPAD_DOWN,
		DPAD_LEFT,
		BUTTON_L2,
		BUTTON_R2,
		BUTTON_L1,
		BUTTON_R1,
		BUTTON_TRIANGLE,
		BUTTON_CIRCLE,
		BUTTON_CROSS,
		BUTTON_SQUARE,
		BUTTON_TOUCH_PAD,
		BUTTON_INTERCEPTED,
	};

	//std::unordered_map<char*, int> Actions;

	int Initialize();
	void Update();
	bool KeyDown(int key);
	bool KeyUp(int key);
	bool KeyPressed(int key);
	bool KeyReleased(int key);
	bool ButtonDown(int key);
	bool ButtonUp(int key);
	bool ButtonPressed(int key);
	bool ButtonReleased(int key);

	bool IsControllerConnected();

	float GetLStickX();
	float GetLStickY();
	float GetRStickX();
	float GetRStickY();

	float GetLStickMagnitude();
	float GetRStickMagnitude();

	void UpdateLStick();
	void UpdateRStick();
};