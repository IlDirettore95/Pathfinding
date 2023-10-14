#include "Profiling.h"

namespace Profiling
{
	namespace
	{
		std::unordered_map<const char*, double> TimeStamps;
		std::unordered_map<const char*, int> CallNumbers;
	}

	Timer::Timer(const char* observedName)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
		m_observedName = observedName;
	}

	Timer::~Timer()
	{
		Stop();
	}

	void Timer::Stop()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint = std::chrono::high_resolution_clock::now();

		int64_t start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimePoint).time_since_epoch().count();
		int64_t end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();

		int64_t duration = end - start;

		double ms = duration * 0.000001;

		TimeStamps[m_observedName] = ms;
		CallNumbers[m_observedName] = 1;
	}

	void PrintTimeStamps()
	{
		const int numberOfSpacesPerName = 20;

		for (const std::pair<const char*, double> entry : TimeStamps)
		{
			std::string name = entry.first;
			std::string spaces = "";
			for (int i = 0; i < numberOfSpacesPerName - name.size(); i++)
			{
				spaces.append(" ");
			}
			std::cout << name << spaces <<"\t(took " << entry.second << "ms)   " << "\t(called " << CallNumbers[entry.first] << " times)" << std::endl;
		}		
	}

	AccumulatedTimer::AccumulatedTimer(const char* observedName)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
		m_observedName = observedName;

		TimeStamps[m_observedName] = TimeStamps.contains(m_observedName) ? TimeStamps[m_observedName] : 0;
		CallNumbers[m_observedName] = CallNumbers.contains(m_observedName) ? CallNumbers[m_observedName] + 1 : 1;
	}

	AccumulatedTimer::~AccumulatedTimer()
	{
		Stop();
	}

	void AccumulatedTimer::Stop()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint = std::chrono::high_resolution_clock::now();

		int64_t start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimePoint).time_since_epoch().count();
		int64_t end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();

		int64_t duration = end - start;

		double ms = duration * 0.000001;

		TimeStamps[m_observedName] += ms;
	}
}