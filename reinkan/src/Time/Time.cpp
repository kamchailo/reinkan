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

		deltaTime = timeElapse - lastFrame;
	}

	double Time::GetElapseTime() const
	{
		return timeElapse;
	}

	double Time::GetDeltaTime() const
	{
		return deltaTime;
	}

	double Time::GetCurrentTIme() const
	{
		return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
}
