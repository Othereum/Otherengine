#pragma once
#include "Core.hpp"
#include "JsonFwd.hpp"
#include "Templates/String.hpp"

namespace oeng
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
		Name(const std::string& s);
		Name(std::string&& s);
		Name(const char* s);

		operator const std::string&() const noexcept { return *sp; }
		const std::string& operator*() const noexcept { return *sp; }
		const std::string* operator->() const noexcept { return sp; }

		bool operator==(const Name& r) const noexcept { return sp == r.sp; }
		bool operator!=(const Name& r) const noexcept { return sp != r.sp; }
		bool operator<(const Name& r) const noexcept { return sp < r.sp; }
		bool operator>(const Name& r) const noexcept { return sp > r.sp; }
		bool operator<=(const Name& r) const noexcept { return sp <= r.sp; }
		bool operator>=(const Name& r) const noexcept { return sp >= r.sp; }

	private:
		const std::string* sp;
	};
	
	void to_json(Json& json, const Name& name);
	void from_json(const Json& json, Name& name);
}

template <>
struct std::hash<oeng::Name>
{
	size_t operator()(const oeng::Name& key) const noexcept
	{
		return size_t(&*key);
	}
};
