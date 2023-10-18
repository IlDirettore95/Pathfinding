#pragma once

#include <chrono>
#include <iostream>
#include <unordered_map>

#if defined GMDG_PROFILE || defined GMDG_DEBUG
#define START_PROFILE() Profiling::AccumulatedTimer timer(__FUNCTION__, __FUNCSIG__);
#define PRINT_PROFILING_DATA() Profiling::PrintTimeStamps(); 
#else
#define START_PROFILE()
#define PRINT_PROFILING_DATA()
#endif // GMDG_PROFILING

namespace Profiling
{
	class ProfilingData
	{
	private:
		const char* m_functionName;
		std::vector<int64_t> m_starts;
		std::vector<int64_t> m_ends;
	public:
		ProfilingData();
		ProfilingData(const char* functionName);
		const char* GetFunctionName();
		ProfilingData* AddTimeStamp(int64_t start, int64_t end);
		std::vector<int64_t>* GetStarts();
		std::vector<int64_t>* GetEnds();
		size_t GetCallCount();
	};

	class AccumulatedTimer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
		const char* m_functionName;
		const char* m_functionSign;
	public:
		AccumulatedTimer(const char* functionName, const char* functionSign);
		~AccumulatedTimer();
		void Stop();
	};

	void PrintTimeStamps();
}