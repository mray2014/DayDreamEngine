#include "DreamInput.h"
#include "DreamMath.h"
#include <Windows.h>
#include <Xinput.h>

using namespace DreamInput;
bool allKeys[256];

struct ThumbStick {
	float x;
	float y;
	float magnitude;
	float normalizedX;
	float normalizedY;
	float normalizedMagnitude;
	float deadZone;
};

volatile ThumbStick LeftStick = {};
volatile ThumbStick RightStick = {};

float deadzone;

uint32_t Button[]
{
	XINPUT_GAMEPAD_LEFT_THUMB, /*BUTTON_L3*/
	XINPUT_GAMEPAD_RIGHT_THUMB, /*BUTTON_R3*/
	XINPUT_GAMEPAD_START, /*BUTTON_OPTIONS*/
	XINPUT_GAMEPAD_DPAD_UP, /*BUTTON_UP*/
	XINPUT_GAMEPAD_DPAD_RIGHT, /*BUTTON_RIGHT*/
	XINPUT_GAMEPAD_DPAD_DOWN, /*BUTTON_DOWN*/
	XINPUT_GAMEPAD_DPAD_LEFT, /*BUTTON_LEFT*/
	0x0, /*BUTTON_L2*/
	0x0, /*BUTTON_R2*/
	XINPUT_GAMEPAD_LEFT_SHOULDER, /*BUTTON_L1*/
	XINPUT_GAMEPAD_RIGHT_SHOULDER, /*BUTTON_R1*/
	XINPUT_GAMEPAD_Y, /*BUTTON_TRIANGLE*/
	XINPUT_GAMEPAD_B, /*BUTTON_CIRCLE*/
	XINPUT_GAMEPAD_A, /*BUTTON_CROSS*/
	XINPUT_GAMEPAD_X, /*BUTTON_SQUARE*/
	XINPUT_GAMEPAD_BACK, /*BUTTON_TOUCH_PAD*/
	XINPUT_GAMEPAD_BACK, /*BUTTON_INTERCEPTED*/
};

XINPUT_STATE previousData;
XINPUT_STATE currentData;

uint32_t m_pressedButtonData = 0x0;
uint32_t m_releasedButtonData = 0x0;

bool controllerConnected;

int DreamInput::Initialize()
{
	LeftStick.deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 2;
	RightStick.deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

	memset(&previousData, 0x0, sizeof(XINPUT_STATE));
	memset(&currentData, 0x0, sizeof(XINPUT_STATE));

	DWORD result = ERROR_ALERTED;
	//result = XInputGetState(0, &currentData);

	if (result == ERROR_SUCCESS)
	{
		controllerConnected = true;
	}
	else
	{
		controllerConnected = false;
	}
	deadzone = 0.2f;
	return 0;
}

void DreamInput::Update()
{
	if (controllerConnected) 
	{
		memcpy(&previousData, &currentData, sizeof(XINPUT_STATE));

		memset(&currentData, 0x0, sizeof(XINPUT_STATE));

		DWORD result = ERROR_ALERTED;
		//result = XInputGetState(0, &currentData);

		if (result == ERROR_SUCCESS)
		{
			m_pressedButtonData = (currentData.Gamepad.wButtons & ~previousData.Gamepad.wButtons);
			m_releasedButtonData = (~currentData.Gamepad.wButtons & previousData.Gamepad.wButtons);
			UpdateLStick();
			UpdateRStick();
		}
		else
		{
			controllerConnected = false;
		}
	}
	else
	{
		DWORD result = ERROR_ALERTED;
		//result = XInputGetState(0, &currentData);

		if (result == ERROR_SUCCESS)
		{
			controllerConnected = true;
			m_pressedButtonData = (currentData.Gamepad.wButtons & ~previousData.Gamepad.wButtons);
			m_releasedButtonData = (~currentData.Gamepad.wButtons & previousData.Gamepad.wButtons);
			UpdateLStick();
			UpdateRStick();
		}
	}
}
bool DreamInput::KeyDown(int key)
{
	if (GetKeyState(key) & 0x8000) {
		allKeys[key] = true;
		return true;
	}
	else {
		allKeys[key] = false;
		return false;
	}
}
bool DreamInput::KeyUp(int key)
{
	if (KeyDown(key)) {
		return false;
	}
	else {
		return true;
	}
}
bool DreamInput::KeyPressed(int key)
{
	if (!allKeys[key]) {
		if (KeyDown(key)) {
			return true;
		}
		return false;
	}
	else {
		KeyDown(key);
		return false;
	}
}
bool DreamInput::KeyReleased(int key)
{
	if (allKeys[key]) {
		if (!KeyDown(key)) {
			return true;
		}
		return false;
	}
	else {
		KeyDown(key);
		return false;
	}
}
bool DreamInput::ButtonDown(int key)
{
	if (key != 7 && key != 8)
	{
		uint32_t newKey = Button[key];
		if ((currentData.Gamepad.wButtons & newKey) == newKey) {
			return true;
		}
		else {
			return false;
		}
	}
	else
	{
		return key == 7 ? currentData.Gamepad.bLeftTrigger > 0.0f : currentData.Gamepad.bRightTrigger > 0.0f;
	}
}

bool DreamInput::ButtonUp(int key)
{
	if(key != 7 && key != 8)
	{
		uint32_t newKey = Button[key];
		if ((currentData.Gamepad.wButtons & newKey) == newKey) {
			return false;
		}
		else {
			return true;
		}
	}
	else
	{
		return key == 7 ? currentData.Gamepad.bLeftTrigger == 0.0f : currentData.Gamepad.bRightTrigger == 0.0f;
	}
}

bool DreamInput::ButtonPressed(int key)
{
	if (key != 7 && key != 8)
	{
		uint32_t newKey = Button[key];
		if ((m_pressedButtonData & newKey) == newKey) {
			return true;
		}
		else {
			return false;
		}
	}
	else
	{
		return key == 7 ? (previousData.Gamepad.bLeftTrigger == 0.0f && currentData.Gamepad.bLeftTrigger > 0.0f) : (previousData.Gamepad.bRightTrigger == 0.0f && currentData.Gamepad.bRightTrigger > 0.0f);
	}
}

bool DreamInput::ButtonReleased(int key)
{
	if (key != 7 && key != 8)
	{
		uint32_t newKey = Button[key];
		if ((m_releasedButtonData & newKey) == newKey) {
			return true;
		}
		else {
			return false;
		}
	}
	else
	{
		return key == 7 ? (previousData.Gamepad.bLeftTrigger != 0.0f && currentData.Gamepad.bLeftTrigger == 0.0f) : (previousData.Gamepad.bRightTrigger != 0.0f && currentData.Gamepad.bRightTrigger == 0.0f);
	}
}

bool DreamInput::IsControllerConnected()
{
	return controllerConnected;
}

float DreamInput::GetLStickX()
{
	return LeftStick.normalizedX;
}

float DreamInput::GetLStickY()
{
	return LeftStick.normalizedY;
}

float DreamInput::GetRStickX()
{
	return RightStick.normalizedX;
}

float DreamInput::GetRStickY()
{
	return RightStick.normalizedY;
}

float DreamInput::GetLStickMagnitude()
{
	return LeftStick.normalizedMagnitude;
}

float DreamInput::GetRStickMagnitude()
{
	return RightStick.normalizedMagnitude;
}

void DreamInput::UpdateLStick()
{
	//LeftStick.x = (float)currentData.Gamepad.sThumbLX;
	//LeftStick.y = (float)currentData.Gamepad.sThumbLY;


	////determine how far the controller is pushed
	//LeftStick.magnitude = std::sqrtf((LeftStick.x  * LeftStick.x) + (LeftStick.y * LeftStick.y));

	////determine the direction the controller is pushed
	//LeftStick.normalizedX = (LeftStick.x / LeftStick.magnitude);
	////LeftStick.normalizedX -= 9.0f;
	//LeftStick.normalizedY = LeftStick.y / LeftStick.magnitude;

	//LeftStick.normalizedMagnitude = 0;

	////check if the controller is outside a circular dead zone
	//if (LeftStick.magnitude > LeftStick.deadZone)
	//{
	//	//clip the magnitude at its expected maximum value
	//	if (LeftStick.magnitude > 32767) LeftStick.magnitude = 32767;

	//	//adjust magnitude relative to the end of the dead zone
	//	LeftStick.magnitude -= LeftStick.deadZone;

	//	//optionally normalize the magnitude with respect to its expected range
	//	//giving a magnitude value of 0.0 to 1.0
	//	LeftStick.normalizedMagnitude = LeftStick.magnitude / (32767 - LeftStick.deadZone);
	//}
	//else //if the controller is in the deadzone zero out the magnitude
	//{
	//	LeftStick.normalizedX = 0.0f;
	//	LeftStick.normalizedY = 0.0f;

	//	LeftStick.magnitude = 0.0;
	//	LeftStick.normalizedMagnitude = 0.0;
	//}

	//if(std::abs(LeftStick.normalizedX) < deadzone)
	//{
	//	LeftStick.normalizedX = 0.0f;
	//}
	//if (std::abs(LeftStick.normalizedY) < deadzone)
	//{
	//	LeftStick.normalizedY = 0.0f;
	//}

	LeftStick.x = (float)currentData.Gamepad.sThumbLX;
	LeftStick.y = (float)currentData.Gamepad.sThumbLY;


	//determine how far the controller is pushed
	LeftStick.magnitude = (LeftStick.x  * LeftStick.x) + (LeftStick.y * LeftStick.y);

	//determine the direction the controller is pushed
	LeftStick.normalizedX = LeftStick.x / 32767.0f;
	//LeftStick.normalizedX -= 9.0f;
	LeftStick.normalizedY = LeftStick.y / 32767.0f;

	LeftStick.normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (LeftStick.magnitude > (LeftStick.deadZone * LeftStick.deadZone))
	{
		//clip the magnitude at its expected maximum value
		if (LeftStick.magnitude > (32767 * 32767)) LeftStick.magnitude = 32767.0f * 32767.0f;

		//adjust magnitude relative to the end of the dead zone
		LeftStick.magnitude -= LeftStick.deadZone * LeftStick.deadZone;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		LeftStick.normalizedMagnitude = LeftStick.magnitude / ((32767 * 32767) - (LeftStick.deadZone * LeftStick.deadZone));
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		LeftStick.normalizedX = 0.0f;
		LeftStick.normalizedY = 0.0f;

		LeftStick.magnitude = 0.0;
		LeftStick.normalizedMagnitude = 0.0;
	}

	if (std::abs(LeftStick.normalizedX) < deadzone)
	{
		LeftStick.normalizedX = 0.0f;
	}
	if (std::abs(LeftStick.normalizedY) < deadzone)
	{
		LeftStick.normalizedY = 0.0f;
	}
}
void DreamInput::UpdateRStick()
{
	RightStick.x = (float)currentData.Gamepad.sThumbRX;
	RightStick.y = (float)currentData.Gamepad.sThumbRY;


	//determine how far the controller is pushed
	RightStick.magnitude = DreamMath::sqrtf((RightStick.x  * RightStick.x) + (RightStick.y * RightStick.y));

	//determine the direction the controller is pushed
	RightStick.normalizedX = (RightStick.x / RightStick.magnitude);
	//LeftStick.normalizedX -= 9.0f;
	RightStick.normalizedY = RightStick.y / RightStick.magnitude;

	RightStick.normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (RightStick.magnitude > RightStick.deadZone)
	{
		//clip the magnitude at its expected maximum value
		if (RightStick.magnitude > 32767) RightStick.magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		RightStick.magnitude -= RightStick.deadZone;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		RightStick.normalizedMagnitude = RightStick.magnitude / (32767 - RightStick.deadZone);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		RightStick.normalizedX = 0.0f;
		RightStick.normalizedY = 0.0f;

		RightStick.magnitude = 0.0;
		RightStick.normalizedMagnitude = 0.0;
	}

	if (std::abs(RightStick.normalizedX) < deadzone)
	{
		RightStick.normalizedX = 0.0f;
	}
	if (std::abs(RightStick.normalizedY) < deadzone)
	{
		RightStick.normalizedY = 0.0f;
	}
}
