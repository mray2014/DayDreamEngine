#pragma once
#include "DreamInput.h"
#include "KeyCode.h"
#include <unordered_map>
struct Binding {
	int keyboard;
	int controller;
};
struct MouseScreen
{
	float x;
	float y;
};
class InputManager {
public:

	static InputManager* GetInstance();
	static void Release();
	static void Update();

	static bool IsControllerConnected();

	static float GetLeftStickX();
	static float GetLeftStickY();
	static float GetRightStickX();
	static float GetRightStickY();

	static bool IsKeyUp(int key);
	static bool IsKeyDown(int key);
	static bool IsKeyPressed(int key);
	static bool IsKeyReleased(int key);

	static bool IsButtonUp(int key);
	static bool IsButtonDown(int key);
	static bool IsButtonPressed(int key);
	static bool IsButtonReleased(int key);

	static bool IsActionUp(int act);
	static bool IsActionDown(int act);
	static bool IsActionPressed(int act);
	static bool IsActionReleased(int act);

	static void AddActionBinding(int act, Binding bind);
	static void AddActionBinding(int act, int keyboardBind, int controllerBind);
	static void ChangeActionBinding(int act, Binding bind);
	static void ChangeActionBinding(int act, int keyboardBind, int controllerBind);
	static void ChangeActionKeyBoardBinding(int act, int keyboardBind);
	static void ChangeActionControllerBinding(int act, int controllerBind);

	MouseScreen currentMousePos;
	bool click;
	bool prevClick;
private:
	InputManager();
	static InputManager* instance;
};