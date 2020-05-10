#pragma once
#include <map>
#include <string>
#include <vector>

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
	using Json = nlohmann::basic_json<std::map, std::vector, std::string, bool, intptr_t, uintptr_t, float, std::allocator, nlohmann::adl_serializer>;
}
