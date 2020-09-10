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

NameSet& GetNameSet() noexcept
{
    if constexpr (!kNameThreadSafe)
    {
        assert(IsGameThread());
    }

    static NameSet set{std::u8string{}};
    return set;
}

Name::Name() noexcept : sp_{&*GetNameSet()->find(std::u8string{})}
{
}

Name::Name(std::u8string&& s) : sp_{&*GetNameSet()->insert(std::move(s)).first}
{
}

Name::Name(const std::u8string& s) : sp_{&*GetNameSet()->insert(s).first}
{
}

// Some compilers do not support is_transparent
template <class Container> concept Transparent = requires(Container c)
{
    c.find(std::declval<std::u8string_view>());
};

template <Transparent Container>
[[nodiscard]] static decltype(auto) Find(const Container& container, std::u8string_view key)
{
    return container.find(key);
}

template <class Container>[[nodiscard]] static decltype(auto) Find(const Container& container, std::u8string_view key)
{
    return container.find(std::u8string{key});
}

Name::Name(std::u8string_view s)
{
    auto set = GetNameSet().Lock();
    auto found = Find(*set, s);
    if (found == set->end())
        found = set->emplace(s).first;
    sp_ = &*found;
}
} // namespace core
} // namespace oeng
