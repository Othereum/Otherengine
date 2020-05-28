#include "Assert.hpp"
#include <iostream>

namespace oeng::detail
{
#if OE_BUILD_MODE == 0 || OE_BUILD_MODE == 1

	void OnAssertionFailed(const char* msg) noexcept
	{
		std::cerr << "[FATAL] " << msg << std::endl;
		std::terminate();
	}
	
#endif

	void OnEnsureFailed(const char* msg) noexcept
	{
		std::cerr << "[WARNING] " << msg << std::endl;
	}
}
