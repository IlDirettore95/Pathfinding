#include "Profiling.h"

namespace Profiling
{
	namespace
	{
		std::unordered_map<const char*, ProfilingData> ProfilingDatabase;
	}

	ProfilingData::ProfilingData()
		: m_functionName("") {}

	ProfilingData::ProfilingData(const char* funtionName)
		: m_functionName(funtionName) {}
	
	ProfilingData* ProfilingData::AddTimeStamp(int64_t start, int64_t end)
	{
		m_starts.push_back(start);
		m_ends.push_back(end);

		return this;
	}

	const char* ProfilingData::GetFunctionName()
	{
		return m_functionName;
	}

	std::vector<int64_t>* ProfilingData::GetStarts()
	{
		return &m_starts;
	}
	
	std::vector<int64_t>* ProfilingData::GetEnds()
	{
		return &m_ends;
	}

	size_t ProfilingData::GetCallCount()
	{
		return m_starts.size();
	}

	AccumulatedTimer::AccumulatedTimer(const char* functionName, const char* functionSign)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
		m_functionName = functionName;
		m_functionSign = functionSign;
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

		ProfilingDatabase[m_functionSign] = ProfilingDatabase.contains(m_functionSign) ? *(ProfilingDatabase[m_functionSign].AddTimeStamp(start, end)) : *(ProfilingData(m_functionName).AddTimeStamp(start, end));

		//std::cout << m_functionName << " " << start << std::endl;
	}

	void PrintTimeStamps()
	{
		std::vector<ProfilingData> database;

		for (const std::pair<const char*, ProfilingData> entry : ProfilingDatabase)
		{
			database.push_back(entry.second);
		}

		size_t size = database.size();

		for (int i = 0; i < size - 1; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				if (database[j].GetStarts()->at(0) <= database[i].GetStarts()->at(0))
				{
					ProfilingData temp = database[i];
					database[i] = database[j];
					database[j] = temp;
				}
			}
		}

		ProfilingData data;
		
		char* nTabs = new char[size];
		for (int i = 0; i < size; i++)
		{
			nTabs[i] = 0;
		}

		for (int i = 1; i < size; i++)
		{
			bool isAncestor = false;
			for (int j = i - 1; j >= 0; j--)
			{
				if (database[i].GetStarts()->at(0) < database[j].GetEnds()->at(0))
				{
					nTabs[i] = nTabs[j] + 1;
					isAncestor = true;
					break;
				}
			}

			if (!isAncestor)
				nTabs[i] = 0;
		}

		for (int i = 0; i < size; i++)
		{
			data = database[i];
			
			const char* name = data.GetFunctionName();
			
			double duration = 0;
			for (int j = 0; j < data.GetStarts()->size(); j++)
			{
				int64_t start = data.GetStarts()->at(j);
				int64_t end = data.GetEnds()->at(j);
				duration += (end - start) * 0.000001;
			}
			
			size_t callCount = data.GetCallCount();

			// Tabs
			std::string tabs = "";
			for (int j = 0; j < nTabs[i]; j++)
			{
				tabs.append("\t");
			}

			std::cout << tabs << name << "\t(took " << duration << "ms)   " << "\t(called " << callCount << " times)" << std::endl;
		}
	}
}