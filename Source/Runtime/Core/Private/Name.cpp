#include "Name.hpp"
#include "Templates/Sync.hpp"
#include <unordered_set>

namespace oeng
{
inline namespace core
{
#ifdef OE_NAME_THREADSAFE
constexpr auto kNameThreadSafe = true;
#else
constexpr auto kNameThreadSafe = false;
#endif

struct NameHasher
{
    using is_transparent = std::true_type;

    [[nodiscard]] size_t operator()(std::u8string_view s) const noexcept
    {
        return HashRange(s.begin(), s.end(), tolower);
    }
};

struct NameEqual
{
    using is_transparent = std::true_type;

    [[nodiscard]] bool operator()(std::u8string_view s1, std::u8string_view s2) const noexcept
    {
        return StrEqCi(s1, s2);
    }
};

using NameSetType = std::unordered_set<std::u8string, NameHasher, NameEqual>;
using NameSet = CondMonitor<NameSetType, kNameThreadSafe>;

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

auto Find(const NameSetType& set, std::u8string_view s)
{
    if constexpr (IsTransparent<NameSetType, std::u8string_view>::value)
    {
        return set.find(s);
    }
    else
    {
        return set.find(s.data());
    }
}

NameSet& GetNameSet() noexcept
{
    if constexpr (!kNameThreadSafe)
    {
        assert(IsGameThread());
    }

    static NameSet set{std::u8string{}};
    return set;
}

Name::Name() noexcept
    : sp_{&*GetNameSet()->find(std::u8string{})}
{
}

Name::Name(std::u8string&& s)
    : sp_{&*GetNameSet()->insert(std::move(s)).first}
{
}

Name::Name(const std::u8string& s)
    : sp_{&*GetNameSet()->insert(s).first}
{
}

Name::Name(std::u8string_view s)
{
    auto set = GetNameSet().Lock();
    auto found = Find(*set, s);
    if (found == set->end())
        found = set->emplace(s).first;
    sp_ = &*found;
}
}
}
