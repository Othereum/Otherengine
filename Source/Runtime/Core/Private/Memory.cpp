#include "Memory.hpp"
#include "EngineBase.hpp"

namespace oeng::core
{
	omem::MemoryPoolManager& GetMemPool() noexcept
	{
		return EngineBase::Get().GetMemPool();
	}
}
