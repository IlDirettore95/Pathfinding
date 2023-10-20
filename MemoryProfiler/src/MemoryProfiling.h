#pragma once

#include <memory>
#include <string>

namespace MemoryProfiling
{
	struct AllocationMetrics
	{
		uint64_t TotalAllocated = 0;
		uint64_t  MaxUsage = 0;
		uint64_t GlobalMaxUsage = 0;
		std::string FunctionName;

		AllocationMetrics();
		AllocationMetrics(std::string functionName);
		void AddAllocation(uint64_t size);
	};

	class MemorySessionManager
	{
	public:
		MemorySessionManager(const char* fileName);
		~MemorySessionManager();
	private:
		const char* m_fileName;
	};

	class MemoryTracker
	{
	public:
		MemoryTracker(const char* fSign, const char* fName);
		~MemoryTracker();
	private:
		const char* m_fName;
		const char* m_fSign;
	};

	void WriteToFile(const char* text, void* address, std::size_t count);
	void WriteToFile(const char* text, void* address);

	void PrintResults();
}

#if defined GMDG_MEM_PROFILE || defined GMDG_DEBUG

#define MEMORY_PROFILER_SESSION(fileName) MemoryProfiling::MemorySessionManager sessionManager(fileName);
#define PROFILE_MEMORY() MemoryProfiling::MemoryTracker tracker(__FUNCSIG__, __FUNCTION__);
#define PRINT_MEMORY_PROFILER_RESULTS() MemoryProfiling::PrintResults();

#else

#define MEMORY_PROFILER_SESSION(fileName) 
#define PROFILE_MEMORY()
#define PRINT_MEMORY_PROFILER_RESULTS()

#endif