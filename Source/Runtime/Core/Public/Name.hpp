#pragma once
#include "JsonFwd.hpp"
#include "otm/Hash.hpp"
#include "Templates/HashSet.hpp"
#include "Templates/String.hpp"
#include "Templates/Sync.hpp"

namespace oeng::core
{	
	/**
	 * Lightweight string.
	 * Very fast O(1) copy and comparison.
	 * No heap allocation on copy.
	 * Good to use as key for Map/Set.
	 * @note Case-insensitive.
	 * @note Comparisons are not lexical.
	 */
	struct CORE_API Name
	{
		Name() noexcept;
		Name(std::u8string_view s);
		Name(String8&& s);
		Name(const String8& s);

		operator const String8&() const noexcept { return *sp; }
		const String8& operator*() const noexcept { return *sp; }
		const String8* operator->() const noexcept { return sp; }

		bool operator==(const Name& r) const noexcept { return sp == r.sp; }
		bool operator!=(const Name& r) const noexcept { return sp != r.sp; }
		bool operator<(const Name& r) const noexcept { return sp < r.sp; }
		bool operator>(const Name& r) const noexcept { return sp > r.sp; }
		bool operator<=(const Name& r) const noexcept { return sp <= r.sp; }
		bool operator>=(const Name& r) const noexcept { return sp >= r.sp; }

	private:
		const String8* sp;
	};
	
	struct NameHasher
	{
		using is_transparent = void;
		
		[[nodiscard]] size_t operator()(std::u8string_view s) const noexcept
		{
			return HashRange(s.begin(), s.end(), tolower);
		}
	};

	struct NameEqual
	{
		using is_transparent = void;
		
		[[nodiscard]] bool operator()(std::u8string_view s1, std::u8string_view s2) const noexcept
		{
			return StrEqCi(s1, s2);
		}
	};

#ifdef OE_NAME_THREADSAFE
	constexpr auto kNameThreadSafe = true;
#else
	constexpr auto kNameThreadSafe = false;
#endif
	
	using NameSet = CondMonitor<HashSet<String8, NameHasher, NameEqual>, kNameThreadSafe>;
	
	CORE_API void to_json(Json& json, const Name& name);
	CORE_API void from_json(const Json& json, Name& name);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
struct std::hash<oeng::Name>
{
	size_t operator()(const oeng::Name& key) const noexcept
	{
		return size_t(&*key);
	}
};
#endif
