#pragma once

namespace Reinkan::Time
{
	class Time
	{
	public:
		Time();

		void Update();

		double GetElapseTime() const;

		double GetDeltaTime() const;

		uint32_t GetFPS() const;

		uint32_t GetStableFPS() const;

	private:

		double GetCurrentTIme() const;

		double startTime{ 0.0 };

		double timeElapse{ 0.0 };

		double deltaTime{ 0.0 };

		uint32_t fps;

		uint32_t timeElapseSecond;

		uint32_t stableFps;

		uint32_t lastTimeElapseSecond;
	};
}