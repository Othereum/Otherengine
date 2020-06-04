#pragma once
#include <compare>
#include "JsonFwd.hpp"
#include "API.hpp"
#include "Templates/String.hpp"

namespace oeng
{
	/**
	 * \brief Lightweight string
	 * \brief Very fast O(1) copy and comparison
	 * \brief No heap allocation on copy
	 * \brief Good to use as key for HashMap
	 * \note Case-insensitive
	 */
	struct OEAPI Name
	{
		Name() noexcept;
		Name(const char* s);
		Name(String s);

		[[nodiscard]] const String& Str() const noexcept { return *sp; }
		[[nodiscard]] const char* CStr() const noexcept { return sp->c_str(); }

		std::strong_ordering operator<=>(const Name&) const noexcept = default;

	private:
		friend std::hash<Name>;
		const String* sp;
	};
	
	void to_json(Json& json, const Name& name);
	void from_json(const Json& json, Name& name);
}

template <>
struct std::hash<oeng::Name>
{
	size_t operator()(const oeng::Name& key) const noexcept
	{
		return size_t(key.sp);
	}
};
