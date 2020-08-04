#pragma once
#undef JSON_USE_IMPLICIT_CONVERSIONS
#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <filesystem>
#include <nlohmann/json.hpp>
#include "Math.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/TreeMap.hpp"

CORE_BEGIN

namespace fs = std::filesystem;

using Json = nlohmann::basic_json<TreeMap, DyArr, std::string, bool, int64_t, uint64_t, Float, PoolAllocator, nlohmann::adl_serializer, DyArr<std::uint8_t>>;
using JsonType = nlohmann::detail::value_t;

/**
* Parse json file
* @param file File path
* @return Parsed json object
* @throw std::runtime_error If failed to open or parse
*/
CORE_API Json ReadFileAsJson(const fs::path& file);

CORE_END

namespace otm
{
	template <class T, size_t L>
	void to_json(oeng::Json& json, const Vector<T, L>& v)
	{
		json = v.data;
	}

	template <class T, size_t L>
	void from_json(const oeng::Json& json, Vector<T, L>& v)
	{
		json.get_to(v.data);
	}

	template <class T, size_t Row, size_t Col>
	void to_json(oeng::Json& json, const Matrix<T, Row, Col>& m)
	{
		for (auto& v : m) json.emplace_back(v);
	}

	template <class T, size_t Row, size_t Col>
	void from_json(const oeng::Json& json, Matrix<T, Row, Col>& m)
	{
		for (size_t i=0; i<Row; ++i) from_json(json.at(i), m[i]);
	}
}
