#pragma once

class DreamTimeManager {
public:
	static void Init();
	static void Update();

	static float timeScale;
	static float totalTime;
	static float deltaTime;

};