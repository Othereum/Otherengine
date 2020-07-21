#pragma once
#include "Templates/TreeMap.hpp"
#include "Templates/DyArr.hpp"
#include "MathFwd.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace nlohmann
{
	template<typename T, typename SFINAE>
	struct adl_serializer;

	template<
		template<typename U, typename V, typename... Args> class ObjectType,
		template<typename U, typename... Args> class ArrayType,
		class StringType, class BooleanType,
		class NumberIntegerType,
		class NumberUnsignedType,
		class NumberFloatType,
		template<typename U> class AllocatorType,
		template<typename T, typename SFINAE> class JSONSerializer,
		class BinaryType>
	class basic_json;

	namespace detail
	{
		enum class value_t : std::uint8_t;
	}
}
#endif

namespace oeng::core
{
	using Json = nlohmann::basic_json<TreeMap, DyArr, std::string, bool, int64_t, uint64_t, Float, PoolAllocator, nlohmann::adl_serializer, DyArr<std::uint8_t>>;
	using JsonType = nlohmann::detail::value_t;
}
