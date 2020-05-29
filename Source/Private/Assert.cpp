#include "Assert.hpp"
#include <iostream>

#if OE_BUILD_MODE == 0 || OE_BUILD_MODE == 1

namespace oeng::detail
{
	void OnAssertionFailed(const char* msg) noexcept
	{
		std::cerr << "[FATAL] " << msg << std::endl;
		std::terminate();
	}
	
	void OnEnsureFailed(const char* msg) noexcept
	{
		std::cerr << "[WARNING] " << msg << std::endl;
	}
}

#endif
