#pragma once
#include "Core.hpp"
#include "JsonFwd.hpp"
#include "Templates/String.hpp"

namespace oeng::core
{	
	/**
	 * \brief Lightweight string.
	 * Very fast O(1) copy and comparison.
	 * No heap allocation on copy.
	 * Good to use as key for Map/Set.
	 * \note Case-insensitive.
	 * \note Comparisons are not lexical.
	 */
	struct OEAPI Name
	{
		Name() noexcept;
		Name(const char8_t* s);
		Name(std::u8string&& s);
		Name(const std::u8string& s);

		operator const std::u8string&() const noexcept { return *sp; }
		const std::u8string& operator*() const noexcept { return *sp; }
		const std::u8string* operator->() const noexcept { return sp; }

		bool operator==(const Name& r) const noexcept { return sp == r.sp; }
		bool operator!=(const Name& r) const noexcept { return sp != r.sp; }
		bool operator<(const Name& r) const noexcept { return sp < r.sp; }
		bool operator>(const Name& r) const noexcept { return sp > r.sp; }
		bool operator<=(const Name& r) const noexcept { return sp <= r.sp; }
		bool operator>=(const Name& r) const noexcept { return sp >= r.sp; }

	private:
		const std::u8string* sp;
	};
	
	OEAPI void to_json(Json& json, const Name& name);
	OEAPI void from_json(const Json& json, Name& name);
}

template <>
struct std::hash<oeng::Name>
{
	size_t operator()(const oeng::Name& key) const noexcept
	{
		return size_t(&*key);
	}
};
