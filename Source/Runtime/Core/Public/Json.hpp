#pragma once
#include "JsonFwd.hpp"
#include <nlohmann/json.hpp>
#include <otm/otmfwd.hpp>
#include "Path.hpp"

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

namespace oeng::core
{
	/**
	 * \brief Parse json file
	 * \param file File path
	 * \return Parsed json object
	 * \throw std::runtime_error If failed to open or parse
	 */
	OEAPI Json ReadFileAsJson(Path file);
}
