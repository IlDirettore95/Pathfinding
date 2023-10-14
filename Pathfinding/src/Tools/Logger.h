#pragma once

#include <iostream>

#ifdef GMDG_DEBUG
#define DEBUG(object) Logger::Print(object);
#else
#define DEBUG(object)
#endif // GMDG_DEBUG


namespace Logger
{
	template <typename T>
	void Print(T object);

	template <typename T>
	void Print(T object)
	{
		std::cout << object << std::endl;
	}
}
