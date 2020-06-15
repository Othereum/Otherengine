#pragma once
#include "JsonFwd.hpp"
#include <nlohmann/json.hpp>
#include <otmfwd.hpp>
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
}

namespace oeng
{
	/**
	 * \brief Parse json file
	 * \param file File path
	 * \return Parsed json object
	 * \throw std::runtime_error If failed to open or parse
	 */
	OEAPI Json ReadFileAsJson(Path file);
}
