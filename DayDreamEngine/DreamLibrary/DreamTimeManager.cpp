#include "DreamTimeManager.h"
#include "DreamMath.h"
#include <chrono>

	float DreamTimeManager::totalTime = 0.0f;
	float DreamTimeManager::deltaTime = 0.0f;
	float DreamTimeManager::timeScale = 1.0f;

	using TimePoint = std::chrono::steady_clock::time_point;

	static TimePoint startTime;
	static TimePoint currentTime;
	static TimePoint prevTime;


	void DreamTimeManager::Init()
	{
		std::chrono::high_resolution_clock clock;
		startTime = clock.now();
		currentTime = startTime;
		prevTime = startTime;
	}

	void DreamTimeManager::Update()
	{
		static std::chrono::high_resolution_clock clock;
		prevTime = currentTime;
		currentTime = clock.now();

		auto time = std::chrono::duration<float>(currentTime - prevTime);

		totalTime = std::chrono::duration<float>(currentTime - startTime).count();
		deltaTime = time.count() * timeScale;
	}
