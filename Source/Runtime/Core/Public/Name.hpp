#pragma once
#include "Json.hpp"
#include "Math.hpp"
#include "Templates/HashSet.hpp"
#include "Templates/String.hpp"
#include "Templates/Sync.hpp"

namespace oeng::core
{	
#ifdef OE_NAME_THREADSAFE
	constexpr auto kNameThreadSafe = true;
#else
	constexpr auto kNameThreadSafe = false;
#endif
	
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

	using NameSet = CondMonitor<HashSet<String8, NameHasher, NameEqual>, kNameThreadSafe>;
	
	template <class Container, class KeyEquivalent>
	struct IsTransparent
	{
	private:
	    template <class C, class = decltype(std::declval<C>().find(std::declval<KeyEquivalent>()))>
	    static std::true_type Test(int);
		
	    template <class C>
	    static std::false_type Test(...);

	public:
	    static constexpr bool value = decltype(Test<Container>(0))::value;
	};

	template <class Set>
	auto Find(const Set& set, std::u8string_view s)
	{
		if constexpr (IsTransparent<Set, std::u8string_view>::value)
		{
			return set.find(s);
		}
		else
		{
			return set.find(s.data());
		}
	}

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
		Name() noexcept :sp{&*Set()->find(String8{})} {}
		Name(String8&& s) :sp{&*Set()->insert(std::move(s)).first} {}
		Name(const String8& s) :sp{&*Set()->insert(s).first} {}

		Name(std::u8string_view s)
		{
			auto set = Set().Lock();
			auto found = Find(*set, s);
			if (found == set->end()) found = set->emplace(s).first;
			sp = &*found;
		}

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
		[[nodiscard]] static NameSet& Set() noexcept;
		const String8* sp;
	};
	
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
