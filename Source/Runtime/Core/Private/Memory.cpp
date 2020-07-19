#include "Memory.hpp"
#include "Interfaces/IEngine.hpp"

namespace oeng::core
{
	omem::MemoryPoolManager& GetMemPool() noexcept
	{
		assert(kIEngine);
		return kIEngine->GetMemPool();
	}
}
