#pragma once
#include "Templates/TreeMap.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/String.hpp"
#include "MathFwd.hpp"

namespace nlohmann
{
	template<typename T, typename SFINAE>
	struct adl_serializer;

	template<template<typename U, typename V, typename... Args> class ObjectType,
	         template<typename U, typename... Args> class ArrayType,
	         class StringType, class BooleanType,
	         class NumberIntegerType,
	         class NumberUnsignedType,
	         class NumberFloatType,
	         template<typename U> class AllocatorType,
	         template<typename T, typename SFINAE> class JSONSerializer>
	class basic_json;

	template<typename BasicJsonType>
	class json_pointer;
}

namespace oeng
{
	using Json = nlohmann::basic_json<TreeMap, DyArr, BasicString<char, std::char_traits<char>, RawAllocator<char>>, bool, intptr_t, uintptr_t, Float, PoolAllocator, nlohmann::adl_serializer>;
}
