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

	private:

		double GetCurrentTIme() const;

		double startTime{ 0.0 };

		double timeElapse{ 0.0 };

		double deltaTime{ 0.0 };
	};
}