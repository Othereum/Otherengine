#pragma once
#include "Json.hpp"
#include "Serialization/Archive.hpp"
#include "Templates/String.hpp"

namespace oeng
{
inline namespace core
{
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
    Name() noexcept;
    Name(std::u8string&& s);
    Name(const std::u8string& s);
    Name(std::u8string_view s);

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

    [[nodiscard]] bool operator==(const Name& r) const noexcept
    {
        return sp_ == r.sp_;
    }

    [[nodiscard]] auto operator<=>(const Name& r) const noexcept
    {
        return sp_ <=> r.sp_;
    }

private:
    const std::u8string* sp_;
};

inline void to_json(Json& json, const Name& name)
{
    json = AsString(*name);
}

inline void from_json(const Json& json, Name& name)
{
    name = AsString8(json.get<std::string>());
}

inline Archive& operator<<(Archive& ar, Name& name)
{
    if (ar.IsLoading())
    {
        std::u8string str;
        ar << str;
        name = std::move(str);
    }
    else
    {
        ar << const_cast<std::u8string&>(*name);
    }
    return ar;
}
}
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
