#pragma once
#include "JsonFwd.hpp"
#include <nlohmann/json.hpp>
#include <otmfwd.hpp>

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
