#include "pch.h"
#include "Time.h"

#include <chrono>

namespace Reinkan::Time
{
	Time::Time()
	{
		startTime = GetCurrentTIme();
	}

	void Time::Update()
	{
		double lastFrame = timeElapse;
		timeElapse = GetCurrentTIme() - startTime;

		lastTimeElapseSecond = timeElapseSecond;
		timeElapseSecond = timeElapse;

		deltaTime = timeElapse - lastFrame;

		fps = 1.0 / deltaTime;

		// Change to average FPS
		if (timeElapseSecond > lastTimeElapseSecond)
		{
			stableFps = 1.0 / deltaTime;
		}
	}

	double Time::GetElapseTime() const
	{
		return timeElapse;
	}

	double Time::GetDeltaTime() const
	{
		return deltaTime;
	}

	uint32_t Time::GetFPS() const
	{
		return fps;
	}

	uint32_t Time::GetStableFPS() const
	{
		return stableFps;
	}

	double Time::GetCurrentTIme() const
	{
		return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
}
