#pragma once
#include <compare>
#include "JsonFwd.hpp"
#include "API.hpp"
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
		using Str = BasicString<char, std::char_traits<char>, RawAllocator<char>>;
		
		Name() noexcept;
		Name(const char* s) :Name{Str{s}} {}
		Name(Str&& s);
		Name(const Str& s);

		operator const Str&() const noexcept { return *sp; }
		const Str& operator*() const noexcept { return *sp; }
		const Str* operator->() const noexcept { return sp; }

		std::strong_ordering operator<=>(const Name&) const noexcept = default;

	private:
		friend std::hash<Name>;
		explicit Name(const Str* s) noexcept :sp{s} {}
		const Str* sp;
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

#define NAME(s) []{static ::oeng::Name n{s};return n;}()
