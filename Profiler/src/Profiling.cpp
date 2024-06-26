#include "Profiling.h"
#include <memory>
#include <unordered_map>

#ifdef GMDG_PROFILE

namespace Profiling
{
	namespace
	{
		std::unordered_map<const char*, ProfilingData> s_profilingDatabase;
		int64_t lastStart = 0;
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
		m_start = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
		m_functionName = functionName;
		m_functionSign = functionSign;

		if (m_start == lastStart)
			m_start += 1;

		lastStart = m_start;
	}

	AccumulatedTimer::~AccumulatedTimer()
	{
		Stop();
	}

	void AccumulatedTimer::Stop()
	{
		int64_t end = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();

		s_profilingDatabase[m_functionSign] = s_profilingDatabase.contains(m_functionSign) ? *(s_profilingDatabase[m_functionSign].AddTimeStamp(m_start, end)) : *(ProfilingData(m_functionName).AddTimeStamp(m_start, end));
	}

	void PrintResults()
	{
		std::vector<ProfilingData> database;

		for (const std::pair<const char*, ProfilingData> entry : s_profilingDatabase)
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
		
		// Tabs
		std::unique_ptr<char[]> nTabs = std::make_unique<char[]>(size);
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

		// Duration & Percentage
		std::unique_ptr<double[]> durations = std::make_unique<double[]>(size);
		std::unique_ptr<double[]> percentages = std::make_unique<double[]>(size);
		for (int i = 0; i < size; i++)
		{
			data = database[i];
			double duration = 0;
			for (int j = 0; j < data.GetStarts()->size(); j++)
			{
				int64_t start = data.GetStarts()->at(j);
				int64_t end = data.GetEnds()->at(j);
				duration += (end - start) * 0.000001;
			}
			durations[i] = duration;

			percentages[i] = 100;
			if (nTabs[i] != 0)
			{
				for (int j = i - 1; j >= 0; j--)
				{
					if (nTabs[j] != nTabs[i] - 1) continue;

					percentages[i] = duration / durations[j] * 100;
					break;
				}
			}
		}

		// Print Time 
		for (int i = 0; i < size; i++)
		{
			data = database[i];
			
			const char* name = data.GetFunctionName();		
			double duration = durations[i];
			double percentage = percentages[i];		
			size_t callCount = data.GetCallCount();

			// Tabs
			std::string tabs = "";
			for (int j = 0; j < nTabs[i]; j++)
			{
				tabs.append("\t");
			}

			std::cout << tabs << name << "\ttook "  << duration << "ms (" << percentage << "%)" << "\tcall count (" << callCount << ")" << std::endl;
		}	

		//Reset Profiling
		s_profilingDatabase.clear();
		lastStart = 0;
	}
}

#endif