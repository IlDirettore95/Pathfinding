#pragma once

#include <chrono>
#include <iostream>

namespace Profiling
{
	class ProfilingData
	{
	public:
		ProfilingData();
		ProfilingData(const char* functionName);
		const char* GetFunctionName();
		ProfilingData* AddTimeStamp(int64_t start, int64_t end);
		std::vector<int64_t>* GetStarts();
		std::vector<int64_t>* GetEnds();
		size_t GetCallCount();
	private:
		const char* m_functionName;
		std::vector<int64_t> m_starts;
		std::vector<int64_t> m_ends;
	};

	class AccumulatedTimer
	{
	public:
		AccumulatedTimer(const char* functionName, const char* functionSign);
		~AccumulatedTimer();
		void Stop();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
		const char* m_functionName;
		const char* m_functionSign;
	};

	void PrintResults();
}

#if defined GMDG_PROFILE
#define PROFILE() Profiling::AccumulatedTimer timer(__FUNCTION__, __FUNCSIG__);
#define PRINT_PROFILING_RESULTS() Profiling::PrintResults(); 

#else

#define PROFILE()
#define PRINT_PROFILING_RESULTS()

#endif