#include "DreamTimeManager.h"
#include "DreamMath.h"
#include <chrono>

namespace DreamTimeManager {

	static float totalTime = 0.0f;
	static float deltaTime = 0.0f;

	using TimePoint = std::chrono::steady_clock::time_point;

	static std::chrono::high_resolution_clock clock;
	static TimePoint startTime;
	static TimePoint currentTime;
	static TimePoint prevTime;

	void Init()
	{
		currentTime = clock.now();
	}

	void Update()
	{
		prevTime = currentTime;
		currentTime = clock.now();

		auto time = std::chrono::duration<float>(currentTime - prevTime);

		deltaTime = time.count();
	}
	float GetDeltaTime()
	{
		return deltaTime;
	}
}