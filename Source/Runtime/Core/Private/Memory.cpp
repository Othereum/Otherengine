#include "Memory.hpp"
#include "EngineBase.hpp"

namespace oeng::core
{
	omem::MemoryPoolManager& GetMemPool() noexcept
	{
		assert(kEngineBase);
		return kEngineBase->GetMemPool();
	}
}
