#pragma once

#include <chrono>
#include <iostream>
#include <unordered_map>

#if defined GMDG_PROFILE || defined GMDG_DEBUG
#define START_PROFILE(observedName) { Profiling::Timer timer(observedName);
#define END_PROFILE }
#define START_ACC_PROFILE(observedName) { Profiling::AccumulatedTimer timer(observedName);
#define END_ACC_PROFILE }
#define PRINT_PROFILING_DATA Profiling::PrintTimeStamps(); 
#else
#define START_PROFILE(observedName)
#define END_PROFILE
#define START_ACC_PROFILE(observedName)
#define END_ACC_PROFILE
#define PRINT_PROFILING_DATA
#endif // GMDG_PROFILING

namespace Profiling
{
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
		const char* m_observedName;
	public:
		Timer(const char* observedName);

		~Timer();

		void Stop();

	};

	class AccumulatedTimer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
		const char* m_observedName;
	public:
		AccumulatedTimer(const char* observedName);

		~AccumulatedTimer();

		void Stop();
	};

	void PrintTimeStamps();
}