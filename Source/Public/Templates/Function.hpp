#pragma once
#include <functional>

namespace oeng
{
	template <class Fty>
	using Function = std::function<Fty>;
}
