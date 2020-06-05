#if OE_BUILD_MODE == 0 || OE_BUILD_MODE == 1

#include "Assert.hpp"
#include "Log.hpp"

namespace oeng::detail
{
	void OnAssertionFailed(const char* msg) noexcept
	{
		log::Critical(msg);
		std::terminate();
	}
	
	void OnEnsureFailed(const char* msg) noexcept
	{
		log::Warn(msg);
	}
}

#endif
