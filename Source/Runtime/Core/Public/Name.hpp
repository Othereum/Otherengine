#pragma once
#include "Json.hpp"
#include "Math.hpp"
#include "Templates/String.hpp"
#include "Templates/Sync.hpp"
#include <unordered_set>

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

using NameSet = CondMonitor<std::unordered_set<std::u8string, NameHasher, NameEqual>, kNameThreadSafe>;

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
 * Good to use as key for map/set.
 * @note Case-insensitive.
 * @note Comparisons are not lexical.
 */
struct CORE_API Name
{
    Name() noexcept
        : sp_{&*Set()->find(std::u8string{})}
    {
    }

    Name(std::u8string&& s)
        : sp_{&*Set()->insert(std::move(s)).first}
    {
    }

    Name(const std::u8string& s)
        : sp_{&*Set()->insert(s).first}
    {
    }

    Name(std::u8string_view s)
    {
        auto set = Set().Lock();
        auto found = Find(*set, s);
        if (found == set->end())
            found = set->emplace(s).first;
        sp_ = &*found;
    }

    operator const std::u8string&() const noexcept
    {
        return *sp_;
    }

    const std::u8string& operator*() const noexcept
    {
        return *sp_;
    }

    const std::u8string* operator->() const noexcept
    {
        return sp_;
    }

    bool operator==(const Name& r) const noexcept
    {
        return sp_ == r.sp_;
    }

    bool operator!=(const Name& r) const noexcept
    {
        return sp_ != r.sp_;
    }

    bool operator<(const Name& r) const noexcept
    {
        return sp_ < r.sp_;
    }

    bool operator>(const Name& r) const noexcept
    {
        return sp_ > r.sp_;
    }

    bool operator<=(const Name& r) const noexcept
    {
        return sp_ <= r.sp_;
    }

    bool operator>=(const Name& r) const noexcept
    {
        return sp_ >= r.sp_;
    }

private:
    [[nodiscard]] static NameSet& Set() noexcept;
    const std::u8string* sp_;
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
