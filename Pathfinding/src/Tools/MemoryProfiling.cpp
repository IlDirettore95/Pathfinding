#include "MemoryProfiling.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include <string>
#include <sstream>

#if defined GMDG_MEM_PROFILE || GMDG_DEBUG

namespace MemoryProfiling
{
	namespace
	{
		std::ofstream s_memoryProfilingFile;
		std::vector<MemoryProfiling::MemoryTracker*> s_activeTrackers;
		std::vector<const char*> s_closedFiles;
		bool canProfile = false;
	}

	AllocationMetrics::AllocationMetrics()
		: FunctionName("") {}

	AllocationMetrics::AllocationMetrics(std::string functionName)
		: FunctionName(functionName) {}

	void AllocationMetrics::AddAllocation(uint64_t  size)
	{
		TotalAllocated += size;
		if (TotalAllocated > MaxUsage)
			MaxUsage = TotalAllocated;
	}

	MemorySessionManager::MemorySessionManager(const char* fileName)
		: m_fileName(fileName)
	{
		if (!s_memoryProfilingFile.is_open())
		{
			s_memoryProfilingFile.open(fileName);
		}
	}

	MemorySessionManager::~MemorySessionManager()
	{
		if (s_memoryProfilingFile.is_open())
		{
			s_memoryProfilingFile.close();
			s_closedFiles.push_back(m_fileName);
		}
	}

	MemoryTracker::MemoryTracker(const char* fSign, const char* fName)
		: m_fSign(fSign), m_fName(fName) 
	{
		canProfile = false;
		if (!s_memoryProfilingFile.is_open())
			return;

		s_activeTrackers.push_back(this);
		canProfile = true;
		s_memoryProfilingFile << "INIT" << "\t" << fSign  << "\t" << fName << std::endl;
		s_memoryProfilingFile.flush();
	}

	MemoryTracker::~MemoryTracker()
	{
		if (!s_memoryProfilingFile.is_open())
			return;

		for (int i = 0; i < s_activeTrackers.size(); i++)
		{
			if (s_activeTrackers[i]->m_fSign != this->m_fSign)
				continue;
			s_activeTrackers.erase(s_activeTrackers.begin() + i);
			break;
		}

		s_memoryProfilingFile << "END" << "\t" << m_fSign << "\t" << m_fName << std::endl;
		s_memoryProfilingFile.flush();
	}

	void WriteToFile(const char* text, void* address, std::size_t count)
	{
		if (!s_memoryProfilingFile.is_open()) 
			return;
		if (s_activeTrackers.size() <= 0)
			return;
		if (!canProfile)
			return;
		s_memoryProfilingFile << "\t" << text << "\t" << address << "\t" << count << std::endl;
		s_memoryProfilingFile.flush();
	}

	void WriteToFile(const char* text, void* address)
	{
		if (!s_memoryProfilingFile.is_open())
			return;
		if (s_activeTrackers.size() <= 0)
			return;
		if (!canProfile)
			return;
		s_memoryProfilingFile << "\t" << text << "\t" << address << "\t" << "NULL" << std::endl;
		s_memoryProfilingFile.flush();
	}

	void PrintResults()
	{
		for (int k = 0; k < s_closedFiles.size(); k++)
		{
			std::string line;
			std::ifstream file(s_closedFiles[k]);
			if (!file.is_open())
			{
				std::cout << "Unable to open " << s_closedFiles[k] << std::endl;
				continue;
			}

			AllocationMetrics globalData(s_closedFiles[k]);
			std::unordered_map<std::string, int> allocatedMemory;
			std::unordered_map<std::string, AllocationMetrics> globalMemoryTrackers;
			std::list<std::string> activeFunctions;
			
			while (std::getline(file, line))
			{
				std::vector <std::string> tokens;

				std::istringstream split(line);
				std::string intermediate;

				while (std::getline(split, intermediate, '\t'))
				{
					tokens.push_back(intermediate);
				}

				std::string address;

				for (int i = 0; i < tokens.size(); i++)
				{
					if (tokens[i] == "")
					{
						continue;
					}
					else if (tokens[i] == "INIT")
					{
						i++;
						std::string fSign = tokens[i];
						i++;
						std::string fName = tokens[i];
						if (!globalMemoryTrackers.contains(fSign))
						{
							globalMemoryTrackers[fSign] = AllocationMetrics(fName);
						}
						else
						{
							globalMemoryTrackers[fSign].TotalAllocated = 0;
						}
						activeFunctions.push_back(fSign);
					}
					else if (tokens[i] == "END")
					{
						i++;
						std::string fSign = tokens[i];
						i++;
						std::string fName = tokens[i];
						AllocationMetrics& data = globalMemoryTrackers[fSign];
						data.GlobalMaxUsage = data.MaxUsage > data.GlobalMaxUsage ? data.MaxUsage : data.GlobalMaxUsage;
						activeFunctions.pop_back();
					}
					else if (tokens[i] == "Alloc")
					{
						i++;
						address = tokens[i];
						i++;
						int count = std::stoi(tokens[i]);
						allocatedMemory[address] = count;
						for (std::string fSign : activeFunctions)
						{
							globalMemoryTrackers[fSign].AddAllocation(count);
						}
						globalData.AddAllocation(count);
					}
					else if (tokens[i] == "Free")
					{
						i++;
						address = tokens[i];
						i++;
						int count = 0;
						if (tokens[i] != "NULL")
						{
							count = std::stoi(tokens[i]);
							allocatedMemory.erase(address);
						}
						else
						{
							count = allocatedMemory[address];
						}

						for (std::string fSign : activeFunctions)
						{
							globalMemoryTrackers[fSign].TotalAllocated -= count;
						}
						globalData.TotalAllocated -= count;
					}
				}
			}
			
			for (const std::pair<std::string, AllocationMetrics> entry : globalMemoryTrackers)
			{
				AllocationMetrics data = (AllocationMetrics)entry.second;
				std::cout << data.FunctionName << "\n\tMax Usage: " << data.GlobalMaxUsage * 0.000001 << " Mbytes." << std::endl;
			}
			std::cout << std::endl;
			std::cout << "Global MaxUsage: " << globalData.MaxUsage *  0.000001 << " Mbytes." << std::endl;
			std::cout << "Global Final Memory Leak: " << globalData.TotalAllocated * 0.000001 << " Mbytes." << std::endl;
			file.close();
		}
	}
}

void* operator new(std::size_t count)
{
	void* address = malloc(count);
	MemoryProfiling::WriteToFile("Alloc", address, count);
	return address;
}

void* operator new[](std::size_t count)
{
	void* address = malloc(count);
	MemoryProfiling::WriteToFile("Alloc", address, count);
	return address;
}

void operator delete(void* memory, std::size_t count)
{
	MemoryProfiling::WriteToFile("Free", memory, count);

	free(memory);
}

void operator delete(void* memory)
{
	MemoryProfiling::WriteToFile("Free", memory);
	free(memory);
}

void operator delete[](void* memory, std::size_t count)
{
	MemoryProfiling::WriteToFile("Free", memory, count);
	free(memory);
}

void operator delete[](void* memory)
{
	MemoryProfiling::WriteToFile("Free", memory);
	free(memory);
}

#endif
