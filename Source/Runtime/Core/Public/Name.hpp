#pragma once
#include "JsonFwd.hpp"
#include "otm/Hash.hpp"
#include "Templates/HashSet.hpp"
#include "Templates/String.hpp"
#include "Templates/Sync.hpp"

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
	struct CORE_API Name
	{
		Name() noexcept;
		Name(const char8_t* s);
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
		[[nodiscard]] constexpr size_t operator()(std::u8string_view s) const noexcept
		{
			return HashRange(s.begin(), s.end(), tolower);
		}
	};

	struct NameEqual
	{
		[[nodiscard]] constexpr bool operator()(std::u8string_view s1, std::u8string_view s2) const noexcept
		{
			if (s1.size() != s2.size()) return false;
			for (size_t i = 0; i < s1.size(); ++i)
				if (tolower(s1[i]) != tolower(s2[i]))
					return false;
			return true;
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

template <>
struct std::hash<oeng::Name>
{
	size_t operator()(const oeng::Name& key) const noexcept
	{
		return size_t(&*key);
	}
};
